#include "../Server.hpp"
#include "../Client.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <thread>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <mutex>
#include <stdlib.h>

int reqCount = 0;
mutex mtx;

using namespace std;
void thread_func(string ip, string port, int threadId){
  while(true){
    Client clients;
    clients.build_client(ip,port);
    int socket_fd = clients.get_socket();
    //  cout<<"please input the delay count & the bucket number:"<<endl;
    int delayCount = rand()%3+1;
    int bucketIdx = rand()%512;
    string message = to_string(delayCount)+","+to_string(bucketIdx);
    //getline(cin,message);
    send(socket_fd,message.c_str(),message.size(),0);
    int update_val=0;
    recv(socket_fd,&update_val,sizeof(update_val),MSG_WAITALL);
    if(update_val!=0){
      stringstream ss;
      ss<<"the new value of bucket is "<<update_val<<" threadID: "<<threadId<<"\n";
      cout<<ss.str();
    }
    //close(socket_fd);
  }
}



int main(int argc, char * argv[]){
  if(argc!=3){
    cout<<"wrong argument number!"<<endl;
  }
  int threadNum = 1000;
  vector<thread> threadPool;
  for(int i = 0;i< threadNum;i++){
    threadPool.push_back(thread(thread_func, argv[1],argv[2],i));
  }
  for(int i = 0; i < threadPool.size(); i++) {
    threadPool[i].join();
  }
  return EXIT_SUCCESS;
}
