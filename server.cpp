#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

#include "config_util.hpp"
#include "socket_connect.hpp"

using std::cout;
using std::endl;

int main(int argc, char const* argv[]) {
  config_util::configs configs = config_util::parse_configs(argc, argv);

  // Connect to server port
  socket_connect sc(configs.port);
  sc.connect(true);
  if (sc.not_connected()) {
    perror("connect failed");
    exit(EXIT_FAILURE);
  }

  // Attach socket to port
  int opt = 1;
  if (setsockopt(sc.fd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    perror("setsockopt failed");
    exit(EXIT_FAILURE);
  }

  socklen_t sockaddr_size = sc.sockaddr_size();
  // Bind socket to port
  if (bind(sc.fd(), sc.sockaddr_ptr(), sockaddr_size) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  // Listen to port
  cout << "listening ..." << endl;
  if (listen(sc.fd(), 3) < 0) {
    perror("listen failed");
    exit(EXIT_FAILURE);
  }

  constexpr size_t BUFSIZE = 1024;
  char buffer[BUFSIZE] = {0};
  const char* ack_str = "Ack from server";
  int ack_str_len = strlen(ack_str);

  for (size_t i = 0; i < 3; ++i) {
    // Accept incoming message
    cout << "accepting ... " << i << endl;
    int accept_fd = accept(sc.fd(), sc.sockaddr_ptr(), &sockaddr_size);
    cout << "accept FD [" << accept_fd << "]" << endl;
    if (accept_fd < 0) {
      perror("accept failed");
      exit(EXIT_FAILURE);
    }

    // Read message
    cout << "reading ..." << endl;
    int valread = read(accept_fd, buffer, BUFSIZE);
    cout << "[" << valread << "] " << buffer << endl;

    // Send message
    send(accept_fd, ack_str, ack_str_len, 0);
    cout << "Ack message sent" << endl;

    // Close the accept socket
    close(accept_fd);
  }

  return 0;
}
