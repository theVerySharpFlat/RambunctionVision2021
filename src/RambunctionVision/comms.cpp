#include "RambunctionVision/comms.hpp"

#include <iostream>
#include <exception>

#include <unistd.h>
#include <arpa/inet.h>

namespace rv {

  Request::Request() {
    action = "";
    subject = "";
    key = "";
    value = "";
  }

  Request::Request(std::string request) {
    std::size_t actionPivot = request.find(":");
    std::size_t subjectPivot = request.find(":", actionPivot + 1);
    std::size_t assignmentPivot = request.find("=");

    if (actionPivot != std::string::npos && subjectPivot!= std::string::npos) {
      action = request.substr(0, actionPivot);
      subject = request.substr(actionPivot+1, subjectPivot - (actionPivot+1));
      key = request.substr(subjectPivot+1, assignmentPivot - (subjectPivot+1));
      value = request.substr(assignmentPivot+1);
    }
  }

  std::string Request::asString() {
    return action + ":" + subject + ":" + key + "=" + value;
  }

  Listener::Listener() {
    sockfd = -1;
    address = {0};
  }

  Listener::Listener(int port, int backlog) {
    listen(port, backlog);
  }

  void Listener::startListening(int port, int backlog) {
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) throw "Failed to creat socket with errno: " + std::to_string(errno);
    
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) throw "Failed on setsockopt with errno: " + std::to_string(errno);

    if (bind(sockfd, (struct sockaddr *) &address, sizeof(address)) < 0) throw "Failed to bind socket with errno: " + std::to_string(errno);

    if (listen(sockfd, backlog) < 0) throw "Failed to listen with errno: " + std::to_string(errno);
  }

  Comms Listener::waitForConnection() {
    Comms comms;
    socklen_t length = sizeof(comms.address);

    comms.sockfd = accept(sockfd, (struct sockaddr *) &comms.address, &length);
    if (comms.sockfd < 0) throw "Failed to accept conection with errno: " + std::to_string(errno);

    return comms;
  }

  Comms::Comms() {
    address = {0};
    sockfd = -1;
  }

  Comms::Comms(std::string addressString, int port) {
    connectTo(addressString, port);
  }

  void Comms::connectTo(std::string addressString, int port) {
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    if (inet_pton(AF_INET, addressString.c_str(), &address.sin_addr) <= 0) throw "Invalid address with errno: " + std::to_string(errno);

    if (connect(sockfd, (struct sockaddr *) &address, sizeof(address)) < 0) throw "Failed to connect with errno: " + std::to_string(errno);
  }

  void Comms::send(Request request) {
    std::string requestString = request.asString();

    if (write(sockfd, &requestString, requestString.length()) < 0) throw "Failed to send request with errno: " + std::to_string(errno);
  }

  Request Comms::recive() {
    char buffer[256] = {0};

    if (read(sockfd, &buffer, sizeof(buffer)) < 0) throw "Failed to read request with errno: " + std::to_string(errno);
    return Request(buffer);
  }
}