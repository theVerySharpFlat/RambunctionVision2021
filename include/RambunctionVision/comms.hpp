#ifndef COMMS_HPP
#define COMMS_HPP

#include <string>

#include <sys/socket.h>
#include <netinet/in.h>

namespace rv {

  struct Request {
    std::string action;
    std::string subject;
    std::string key;
    std::string value;
    std::string asString();
  };


  Request parseRequest(std::string request);
  Request error(std::string error, std::string key);

  class Socket {
    protected:
      int sockfd;
      struct sockaddr_in address;

      void creatSocket();

    public:
      Socket();

      void send(Request request);
      Request recive();
      void closeSocket();
  };

  class Server: public Socket {
    public:
    Server();
    Server(int port);

    void bindTo(int port);
    void startListening(int backlog = 5);
    Server waitForConnection();
  };

  class Client: public Socket {
    public:
      Client();
      Client(std::string address, int port);
      
      void connectTo(std::string address, int port);
  };
}
#endif