#include "cg_version.h"
#include "laingcontroller.h"

#include <iostream>

int main()
{
  std::cout << "Cellulite GUI Version: " << CG_VERSION << std::endl;
  auto laingController = LaingController( "/dev/ttyUSB0" );

  return 0;
}