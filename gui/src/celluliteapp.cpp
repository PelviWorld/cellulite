#include "celluliteapp.h"
#include "INIReader.h"
#include "celluliteframe.h"

#include <fstream>

std::ofstream logFile;

void RedirectIOToLogFile( const std::string& logFileName )
{
  logFile.open( logFileName );
  if( logFile.is_open() )
  {
    std::cout.rdbuf( logFile.rdbuf() );
    std::cerr.rdbuf( logFile.rdbuf() );
  }
  else
  {
    std::cerr << "Failed to open log file: " << logFileName << std::endl;
  }
}

bool CelluliteApp::OnInit()
{
  RedirectIOToLogFile( "cellulite.log" );
  if( const INIReader reader( "cellulite.ini" ); reader.ParseError() != 0 )
  {
    std::cout << "Can't load 'cellulite.ini'\n" << std::endl;
  }
  else
  {
    std::cout << "Config value: " << reader.Get( "DEVICES", "one", "NOTHING" ) << std::endl;
    std::cout << "Config value: " << reader.Get( "DEVICES", "two", "NOTHING" ) << std::endl;
    std::cout << "Config value: " << reader.Get( "AXIS", "width", "NOTHING" ) << std::endl;
    std::cout << "Config value: " << reader.Get( "AXIS", "height", "NOTHING" ) << std::endl;
  }
  auto* frame = new CelluliteFrame();
  frame->Show( true );
  return true;
}
LaingController& CelluliteApp::getController( const ControllerAxis axis )
{
  assert( m_controller.contains( axis ) );
  return m_controller[ axis ];
}
