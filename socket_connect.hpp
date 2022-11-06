#pragma once

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

class socket_connect {
  int port_;
  const short in_family_;
  int socket_fd_{ERR_SOCKET};
  sockaddr_in sockaddr_;
  sockaddr* sockaddr_ptr_;
  uint sockaddr_size_;

  static constexpr int ERR_SOCKET = -1;

 public:
  socket_connect(int port, short in_family = AF_INET)
      : port_(port),
        in_family_(in_family),
        sockaddr_ptr_{reinterpret_cast<sockaddr*>(&sockaddr_)},
        sockaddr_size_{sizeof(sockaddr_)} {
    sockaddr_.sin_family = in_family_;
    sockaddr_.sin_port = htons(port_);
  }

  ~socket_connect() {
    // Close the listening socket
    shutdown(socket_fd_, SHUT_RDWR);
  }

  void connect(bool inaddr_any) {
    socket_fd_ = socket(in_family_, SOCK_STREAM, 0);
    std::cout << "socket FD [" << socket_fd_ << "]" << std::endl;
    if (socket_fd_ < 0) {
      std::cout << "Socket creation error" << std::endl;
      socket_fd_ = ERR_SOCKET;
      return;
    }

    if (inaddr_any) {
      sockaddr_.sin_addr.s_addr = INADDR_ANY;
    } else {
      // Convert IPv4 and IPv6 addresses from text to binary form
      if (inet_pton(in_family_, "127.0.0.1", &sockaddr_.sin_addr) <= 0) {
        std::cout << "Invalid address / Address not supported" << std::endl;
        socket_fd_ = ERR_SOCKET;
        return;
      }
    }
  }

  int fd() const { return socket_fd_; }
  bool not_connected() const { socket_fd_ == ERR_SOCKET; }
  sockaddr* sockaddr_ptr() const { return sockaddr_ptr_; }
  uint sockaddr_size() const { return sockaddr_size_; }
};
