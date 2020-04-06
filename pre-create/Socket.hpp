#ifndef __SOCKET_HPP__
#define __SOCKET_HPP__

#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Socket{
protected:
  int socket_fd;
  string hostname;
  string port;
  struct addrinfo host_info;
  struct addrinfo *host_info_list;
  
public:
  Socket(string _hostname, string _port):
    socket_fd(-1),
    hostname(_hostname),
    port(_port),
    host_info_list(NULL)
  {
    memset(&host_info, 0, sizeof(host_info));
    host_info.ai_family = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;
  }

  virtual ~Socket(){
    if(socket_fd > 0){
      close(socket_fd);
    }
    if(host_info_list){
      freeaddrinfo(host_info_list);
    }
  }
  
  virtual void get_addr_info()=0;
  
  int get_socket_fd()const{ return socket_fd; }
};

class Host_Socket : public Socket{
public:
  Host_Socket(string hostname, string port):
    Socket(hostname, port){}
  
  virtual ~Host_Socket(){}
  
  virtual void get_addr_info(){
    host_info.ai_flags = AI_PASSIVE;
    if(getaddrinfo(hostname.c_str(), port.c_str(), &host_info, &host_info_list) != 0){
      cerr << "Cannot get host addr info" << endl;
      exit(EXIT_FAILURE);
    }
  }

  void get_host_fd(){
    socket_fd = -1;
    while(socket_fd == -1){
      socket_fd = socket(host_info_list->ai_family,
                     host_info_list->ai_socktype,
                     host_info_list->ai_protocol);
    }
    /*socket_fd = socket(host_info_list->ai_family, 
		     host_info_list->ai_socktype, 
		     host_info_list->ai_protocol);
    if (socket_fd == -1) {
      cerr << "Error: cannot create socket" << endl;
      cerr << "  (" << hostname << "," << port << ")" << endl;
      exit(EXIT_FAILURE);
      } //if*/

    int yes = 1;
    int status = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    status = bind(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1) {
      cerr << "Error: cannot bind socket" << endl;
      cerr << "  (" << hostname << "," << port << ")" << endl;
      exit(EXIT_FAILURE);
    } //if
  }

  void listen_on_port(){
    int status = listen(socket_fd, 500);
    if (status == -1) {
      cerr << "Error: cannot listen on socket" << endl; 
      cerr << "  (" << hostname << "," << port << ")" << endl;
      exit(EXIT_FAILURE);
    } //if
  }

   int accept_client(){
    int accept_fd;
    struct sockaddr_storage sock_addr;
    socklen_t sock_addr_len = sizeof(sock_addr);
    accept_fd = accept(socket_fd, (struct sockaddr *)&sock_addr, &sock_addr_len);
    return accept_fd;
  }
};

class Client_Socket : public Socket{
public:
  Client_Socket(string hostname, string port):
    Socket(hostname, port){}

  virtual ~Client_Socket(){}
  
  virtual void get_addr_info(){
    int status = 1;
    while(status!=0){
      status = getaddrinfo(hostname.c_str(), port.c_str(), &host_info, &host_info_list);
    }
    /*if(getaddrinfo(hostname.c_str(), port.c_str(), &host_info, &host_info_list) != 0){
      cerr<<"cannot get address info"<<endl;*/
  }
  
  void connect_to_host(){
    socket_fd = -1;
    while(socket_fd==-1){
       socket_fd = socket(host_info_list->ai_family,
                       host_info_list->ai_socktype,
                       host_info_list->ai_protocol);
    }
    /*socket_fd = socket(host_info_list->ai_family, 
		       host_info_list->ai_socktype, 
		       host_info_list->ai_protocol);
    if (socket_fd == -1) {
      cerr << "Error: cannot create socket" << endl;
      cerr << "  (" << hostname << "," << port << ")" << endl;
      exit(EXIT_FAILURE);
      } //if*/
    int status = -1;
    while(status == -1){
      status = connect(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    }
    /*int status = connect(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1) {
      cerr << "Error: cannot connect to socket" << endl;
      cerr << "  (" << hostname << "," << port << ")" << endl;
      exit(EXIT_FAILURE);
      } //if*/
  }
};

#endif














  
