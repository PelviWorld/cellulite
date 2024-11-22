#include "cg_version.h"
#include "laingcontroller.h"
#include "laingvalue.h"

#include <iostream>

int main()
{
  std::cout << "Cellulite GUI Version: " << CG_VERSION << std::endl;
  auto laingController = LaingController( "/dev/ttyUSB0" );
  laingController.moveToUserPosition( AXIS::ONE, USER_POSITION::POS_2 );
  laingController.moveToUserPosition( AXIS::ONE, USER_POSITION::POS_4 );
  laingController.moveToUserPosition( AXIS::ONE, USER_POSITION::POS_1 );
  laingController.moveToUserPosition( AXIS::ONE, USER_POSITION::POS_3 );

  return 0;
}