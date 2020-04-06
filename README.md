#                Project 4: Scalability

## How to run the server:

##### Steps to run the server

After choose the cores to run, type in the following comand to run the server:

```com
sudo docker-compose up --build
```

The server has a timer inside and and will stop atomaticlly after 5 minutes.The server would calculate the requests it handles aloing with the throughput during the 5 minutes and print them on the screen. So to be precise, after running the server, we need to run the client as soon as possible for a more accurate output. 

##### Change the running cores

To change the running cores, take per-create as an example. 

Go to docker-compose.yml in per-create folder, and change this line to whatever kind of combination of cores you want:

```
 cpuset: '0,1,2,3'
```

## How to run the client

##### Steps to run the client

Take per-create as the example. The make file for client is not inside the testing directory, but in the per-create directory. So to run the client successfully, we need to type in

```
make
```

This would execute this line inside the Makefile (under the per-create directory):

```makefile
./testing/client: ./testing/client.cpp Client.hpp Server.hpp Socket.hpp
        g++ -std=c++11 -pthread -O3 -o ./testing/client ./testing/client.cpp
```

which would put an executable file called client into the testing directory.

After having this executable file inside the testing directory, you can go the testing directory and type in the following command to run the client:

```
./client vcm-xxxxx.vm.duke.edu 12345
```

##### Change parameters (delay time & bucket size):

Take per-create as an example:

1. delay time

   Go to ./per-create/testing/client.cpp, and find the following line:

   ```c++
    int delayCount = rand()%3+1;
   ```

   If you would like to chage the delay time to 1--20s, just change the 3 into 20.

   Everytime you change this parameter, you need to rollback to the per-create directory and type in 

   ```
   make
   ```

   Then just run the client as stated before.

2. bucket size

   Go to ./per-create/testing/client.cpp, and find the following line:

   ```c++
   int bucketIdx = rand()%512;
   ```

   If you would like to change the bucket size, just change the 512 into what ever you want. But note that the Dockerfile outside the testing directory also need to be changed. If you want to change the bucket size into 32, after changing the code above into 32, go to ./percreate/Dockerfile and find this line:

   ```dockerfile
   ENTRYPOINT ["./server"]
   CMD ["512"]
   ```

   Alter the 512 into 32 too.

   Every time you change this parameter, you need to rollback to the per-create directory and type in the following two command:

   ```
   make
   sudo docker-compose up --build
   ```

   Then just run the client as stated before immediately (becase the server is already running).

   



