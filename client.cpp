#include <iostream>

#include "config_util.hpp"
#include "socket_connect.hpp"

using std::cout;
using std::endl;

int main(int argc, char const* argv[]) {
  config_util::configs configs = config_util::parse_configs(argc, argv);

  // Connect to server port
  socket_connect sc(configs.port);
  int socket_fd = sc.connect(false);
  if (socket_fd == socket_connect::ERR_SOCKET) {
    perror("connect failed");
    exit(EXIT_FAILURE);
  }

  // Connect client to server port
  int client_fd = connect(socket_fd, sc.sockaddr_ptr(), sc.sockaddr_size());
  if (client_fd < 0) {
    cout << "Connection Failed" << endl;
    return -1;
  }

  // Send message
  const char* hello = "Hello from client";
  send(socket_fd, hello, strlen(hello), 0);
  cout << "Hello message sent" << endl;

  // Read message
  constexpr size_t BUF_SIZE = 1024;
  char buffer[BUF_SIZE] = {0};
  int valread = read(socket_fd, buffer, BUF_SIZE);
  cout << "[" << valread << "] " << buffer << endl;

  // Close the sockets
  close(client_fd);
  close(socket_fd);
  return 0;
}
