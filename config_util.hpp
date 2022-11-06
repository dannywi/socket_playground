#pragma once

#include <iostream>

namespace config_util {
struct configs {
  int port;
};

configs parse_configs(int argc, const char* argv[]) {
  constexpr int DEFAULT_PORT = 8091;
  int port = DEFAULT_PORT;
  if (argc > 1) { port = atoi(argv[1]); }
  std::cout << "\nUsing port [" << port << "]" << std::endl;
  return configs{port};
}
}  // namespace config_util
