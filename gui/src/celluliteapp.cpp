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

auto getValue( const INIReader& reader, const std::string& section, const std::string& key )
{
  auto value = reader.Get( section, key, "-" );
  if( value == "-" )
  {
    std::cout << "Can't find " << key << " in " << section << " section\n" << std::endl;
    throw std::runtime_error( "Can't find " + key + " in " + section + " section" );
  }
  return value;
}

auto getReader( const std::string& filename )
{
  INIReader reader( filename );
  if( reader.ParseError() != 0 )
  {
    std::cout << "Can't load 'cellulite.ini'\n" << std::endl;
    throw std::runtime_error( "Can't load 'cellulite.ini'" );
  }
  return reader;
}

bool CelluliteApp::OnInit()
{
  RedirectIOToLogFile( "cellulite.log" );
  const auto reader = getReader( "cellulite.ini" );

  auto deviceOne = getValue( reader, "DEVICES", "one" );
  auto deviceTwo = getValue( reader, "DEVICES", "two" );

  auto axisWidth = getValue( reader, "AXIS", "width" );
  auto axisHeight = getValue( reader, "AXIS", "height" );

  auto* frame = new CelluliteFrame();
  frame->Show( true );
  return true;
}
LaingController& CelluliteApp::getController( const ControllerAxis axis )
{
  assert( m_controller.contains( axis ) );
  return m_controller[ axis ];
}
