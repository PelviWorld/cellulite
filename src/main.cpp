#include "cg_version.h"
#include "laingcontroller.h"

#include <iostream>

int main()
{
  std::cout << "Cellulite GUI Version: " << CG_VERSION << std::endl;
  auto laingController = LaingController( "/dev/ttyUSB0" );
  laingController.moveToPosition( 2 );
  laingController.moveToPosition( 4 );
  laingController.moveToPosition( 1 );
  laingController.moveToPosition( 3 );

  return 0;
}