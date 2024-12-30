#include "celluliteapp.h"
#include "INIReader.h"
#include "celluliteframe.h"

#include <fstream>

namespace
{
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

  std::string getString( const INIReader& reader, const std::string& section, const std::string& key )
  {
    std::string value = reader.Get( section, key, "-" );
    if( value == "-" )
    {
      std::cout << "Can't find " << key << " in " << section << " section\n" << std::endl;
      throw std::runtime_error( "Can't find " + key + " in " + section + " section" );
    }
    return value;
  }

  int getValue( const INIReader& reader, const std::string& section, const std::string& key )
  {
    const int value = reader.GetInteger64( section, key, 0 );
    if( value == 0 )
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

  std::unordered_map< ControllerAxis, int > readSerialConfig( const INIReader& reader )
  {
    std::unordered_map< ControllerAxis, int > result;

    result[ ControllerAxis::WIDTH ] = getValue( reader, "SERIAL", "width" );
    result[ ControllerAxis::HEIGHT ] = getValue( reader, "SERIAL", "height" );
    if( result.at( ControllerAxis::WIDTH ) == result.at( ControllerAxis::HEIGHT ) )
    {
      std::cout << "Serial numbers are the same" << std::endl;
      throw std::runtime_error( "Serial numbers are the same" );
    }
    return result;
  }

}

bool CelluliteApp::OnInit()
{
  RedirectIOToLogFile( "cellulite.log" );
  try
  {
    createController();
  }
  catch( std::runtime_error )
  {
    std::cout << "Failed to load configuration file" << std::endl;
    return true;
  }

  auto* frame = new CelluliteFrame();
  frame->Show( true );
  return true;
}

std::shared_ptr< LaingController > CelluliteApp::getController( const ControllerAxis axis )
{
  assert( m_controller.contains( axis ) );
  return m_controller[ axis ];
}

void CelluliteApp::createController()
{
  const auto reader = getReader( "cellulite.ini" );
  const auto serialConfig = readSerialConfig( reader );
  createControllerMap( reader, serialConfig );
}

void CelluliteApp::createControllerMap(
  const INIReader& reader, const std::unordered_map< ControllerAxis, int >& serialConfig )
{
  std::string deviceOne = getString( reader, "DEVICES", "one" );
  std::string deviceTwo = getString( reader, "DEVICES", "two" );
  moveControllerToMap( std::make_shared< LaingController >( deviceOne ), serialConfig );
  moveControllerToMap( std::make_shared< LaingController >( deviceTwo ), serialConfig );
}

void CelluliteApp::moveControllerToMap(
  std::shared_ptr< LaingController > controller, const std::unordered_map< ControllerAxis, int >& serialConfig )
{
  const auto controllerSerial = controller->getSerialNumber();
  for( const auto& [ axis, serial ] : serialConfig )
  {
    if( serial == controllerSerial )
    {
      m_controller[ axis ] = std::move( controller );
      return;
    }
  }
}
