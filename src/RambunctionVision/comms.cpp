#include "RambunctionVision/comms.hpp"

#include <iostream>

#include <unistd.h>
#include <arpa/inet.h>

namespace rv {

  std::string Request::asString() {
    return action + ":" + subject + ":" + key + "=" + value;
  }


  Request parseRequest(std::string requestString) {
    Request  request;

    std::size_t actionPivot = requestString.find(":");
    std::size_t subjectPivot = requestString.find(":", actionPivot + 1);
    std::size_t assignmentPivot = requestString.find("=");

    if (actionPivot != std::string::npos && subjectPivot!= std::string::npos && assignmentPivot != std::string::npos) {
      request.action = requestString.substr(0, actionPivot);
      request.subject = requestString.substr(actionPivot+1, subjectPivot - (actionPivot+1));
      request.key = requestString.substr(subjectPivot+1, assignmentPivot - (subjectPivot+1));
      request.value = requestString.substr(assignmentPivot+1);
      return request;
    }
    throw "Invalid Formatt";
  }

  Socket::Socket() {
    creatSocket();
  }

  void Socket::creatSocket() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
      throw "Failed to creat socket";
    }
  }

  void Socket::send(Request request) {
    std::string reqeustString = request.asString();
    if (write(sockfd, &reqeustString, sizeof(reqeustString)) < 0) {
      throw "Failed to write to socket";
    }
  }

  Request Socket::recive() {
    char buffer[256] = {0};
    if (read(sockfd, &buffer, sizeof(buffer)) < 0) {
      throw "Failed to read from socket";
    }
    std::string requestString = buffer;
    return parseRequest(requestString);
  }

  void Socket::closeSocket() {
    close(sockfd);
  }

  Server::Server() {
    creatSocket();
  }

  Server::Server(int socketPort) {
    creatSocket();
    bindTo(socketPort); 
  }

  void Server::bindTo(int socketPort) {
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(socketPort); 
    
    if (bind(sockfd, (struct sockaddr *) &address, sizeof(address)) < 0) {
      throw "Failed to bind socket";
    }
  }

  void Server::startListening(int backlog) {
    if (listen(sockfd, backlog) < 0) {
      throw "Error listening to socket";
    }
  }

  Server Server::waitForConnection() {
    Server connection;
    socklen_t length = sizeof(connection.address);
    connection.sockfd = accept(sockfd, (struct sockaddr *) &connection.address, &length);
    if (connection.sockfd < 0) {
      throw "Failed to accept connection";
    }
    return connection;
  }

  Client::Client() {
    creatSocket();
  }

  Client::Client(std::string serverAddress, int serverPort) {
    creatSocket();
    connectTo(serverAddress, serverPort);
  }

  void Client::connectTo(std::string serverAddress, int serverPort) {
    address.sin_family = AF_INET;
    address.sin_port = htons(serverPort);

    if (inet_pton(AF_INET, serverAddress.c_str(), &address.sin_addr) <= 0) {
      throw "Invalid Address";
    }

    if (connect(sockfd, (struct sockaddr *) &address, sizeof(address)) < 0) {
      throw "Failed to connect to socket";
    }
  }
}