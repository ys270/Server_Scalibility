#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include "Socket.hpp"

using namespace std;

class Server{
private:
  Host_Socket * server;
public:
  Server():server(NULL){}

  ~Server(){delete server;}

  void build_server(string hostname, string port){
    server = new Host_Socket(hostname, port);
    server->get_addr_info();
    server->get_host_fd();
    server->listen_on_port();
  }

  int accept() {
    return server->accept_client();
  }
};

#endif
