#include "Server.hpp"
#include "Client.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <mutex>
#include <queue>

using namespace std;
std::mutex mtx;
queue<int> fds;    // store client file descriptors
int reqCount = 0;

void delay(double req_delay) {
  struct timeval start, check, end;
  double elapsed_seconds;
  gettimeofday(&start, NULL);
  do {
    gettimeofday(&check, NULL);
    elapsed_seconds = (check.tv_sec + (check.tv_usec / 1000000.0)) - (start.tv_sec + (start.tv_usec / 1000000.0));
  } while (elapsed_seconds < req_delay);
}

int addToBucket(vector<int> & vec, int delayCount, int idx) {
  mtx.lock();
  vec[idx] += delayCount;
  int ret = vec[idx];
  reqCount++;
  mtx.unlock();
  return ret;
}

void work(vector<int> & vec, int clientFD) {
  char req[65535];
  int lenRecved = 0;
  do {
    lenRecved = recv(clientFD, req, 65535, 0);
  } while(lenRecved == 0);
  
  string cnt = "";
  for (int i = 0; i < lenRecved; i++) {
    cnt.push_back(req[i]);
  }
  int commaIdx = cnt.find(",");

  // for test
  //  cout << "cnt: " << cnt << endl;

  int delayCount = stoi(cnt.substr(0, commaIdx));
  int bucketIdx = stoi(cnt.substr(commaIdx + 1, cnt.size() - 1 - commaIdx));
  delay(delayCount);   // convert from int to double
  int res = addToBucket(vec, delayCount, bucketIdx);
  //cout << "bucket[" << bucketIdx << "] = " << res << endl;
  send(clientFD, &res, sizeof(res), 0);
}

void thread_func(vector<int> & vec, queue<int> & fds) {
  while (true) {
    int clientFD;
    mtx.lock();
    if (!fds.empty()) {
      clientFD = fds.front();
      fds.pop();
      mtx.unlock();
    }
    else {  // queue is empty
      mtx.unlock();
      continue;
    }
    work(vec, clientFD);
    close(clientFD);
  }
}

int main(int argc, char * argv[]) {
  if (argc != 2) {
    cout << "Wrong argument number!" << endl;
  }

  int len = atoi(argv[1]);
  vector<int> info(len, 0);   // buckets

  // pre-create threads
  //  vector<thread> threadPool;

  int threadNum = 950;
  for (int i = 0; i < threadNum; i++) {
    thread newThread(thread_func, ref(info), ref(fds));
    // threadPool.push_back(newThread);
    newThread.detach();
  }

  Server server;
  server.build_server("0.0.0.0", "12345");

  double timer = 300;    // 5min
  struct timeval start, check, end;
  double elapsed_seconds;
  gettimeofday(&start, NULL);
  do {
    gettimeofday(&check, NULL);
    elapsed_seconds = (check.tv_sec + (check.tv_usec / 1000000.0)) - (start.tv_sec + (start.tv_usec / 1000000.0));
    // actual work
    int tmpFd = server.accept();
    mtx.lock();
    fds.push(tmpFd);
    mtx.unlock();

  } while (elapsed_seconds < timer);

  cout << "*****************\n\n" << endl;
  cout << "REQCOUNT: " << reqCount << endl;
  cout << "THROUGH PUT: " << reqCount / timer << endl;
  cout << "*****************\n\n" << endl;

  return 0;
}
