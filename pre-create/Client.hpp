#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include "Socket.hpp"

using namespace std;

class Client{
private:
  Client_Socket * client;
public:
  Client():client(NULL){}
  
  ~Client(){delete client;}

  void build_client(string hostname, string port){
    client = new Client_Socket(hostname, port);
    client->get_addr_info();
    client->connect_to_host();
  }
  int get_socket(){
    return client->get_socket_fd();
  }
};
#endif
