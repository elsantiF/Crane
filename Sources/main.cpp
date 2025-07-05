#include "Application.hpp"
#include <iostream>

int main() {
  Application crane;
  if (!crane.Initialize()) {
    std::cerr << "Failed to initialize Crane." << std::endl;
    return -1;
  }

  crane.Run();
  return 0;
}