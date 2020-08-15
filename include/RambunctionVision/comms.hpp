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
    Request(std::string action, std::string subject, std::string key, std::string value);
  };

  class Comms {
    private:
      int sockfd;
      struct sockaddr_in address;

      friend class Listener;

      public:
        Comms();
        Comms(std::string address, int port);

        void connectTo(std::string address, int port);   
        void send(Request request);
        Request recive();   
        void closeConnection();
  };

  class Listener {
    private:
      int sockfd;
      struct sockaddr_in address;

    public:
      Listener();
      Listener(int port, int backlog);
     
      void startListening(int port, int backlog);
      Comms waitForConnection();
      void stopListening();
  };
}
#endif