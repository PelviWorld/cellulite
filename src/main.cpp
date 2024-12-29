#include "cg_version.h"
#include "laingcontroller.h"

#include <iostream>

int main()
{
  std::clog << "Cellulite GUI Version: " << CG_VERSION << std::endl;
  auto laingController = LaingController( "COM3" );
  auto laingController2 = LaingController( "COM6" );

  return 0;
}