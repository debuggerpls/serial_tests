#include "controller.hpp"

#include <iostream>

int main(int argc, char** argv)
{
  if (argc < 2)
    return EXIT_FAILURE;

  std::string device(argv[1]);

  Controller port(device);
  if (!port.open())
  {
    std::cerr << "Cannot open port!\n";
    return EXIT_FAILURE;
  }

  while(true)
  {
    port.triggerRead();
    usleep(10000);

    // port.normalRead();
    // sleep(1);
  }

  return 0;

}