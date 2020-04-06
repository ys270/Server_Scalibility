CC=g++
CFLAGS=   #-pedantic -Werror -Wall #-O3 -fPIC  -lpthread                                 
#CFLAGS=-ggdb3 -fPIC

all: server ./testing/client

server: main.cpp Client.hpp Server.hpp Socket.hpp
	g++ -std=c++11 -pthread -O3 -o server main.cpp

./testing/client: ./testing/client.cpp Client.hpp Server.hpp Socket.hpp
	g++ -std=c++11 -pthread -O3 -o ./testing/client ./testing/client.cpp

clean:
	rm -rf server ./testing/client *.o *~


