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

    Request();
    Request(std::string request);
  };

  class Comms {
    private:
      int sockfd;
      struct sockaddr_in address;

      friend class Listener;

      public:
        Comms();
        Comms(std::string address, int port);
        ~Comms();

        void connectTo(std::string address, int port);   
        void send(Request request);
        Request recive();   
  };

  class Listener {
    private:
      int sockfd;
      struct sockaddr_in address;

    public:
      Listener();
      Listener(int port, int backlog);
      ~Listener();
     
      void startListening(int port, int backlog);
      Comms waitForConnection();
  };
}
#endif