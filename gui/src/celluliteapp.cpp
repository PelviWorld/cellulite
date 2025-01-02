#include "celluliteapp.h"
#include "INIReader.h"
#include "celluliteframe.h"
#include "utility.h"

#include <array>

namespace
{
  int getValue( const INIReader& reader, const std::string& section, const std::string& key )
  {
    const int value = reader.GetInteger( section, key, 0 );
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

  void moveControllerToMap( const std::shared_ptr< LaingController >& controller,
    const std::unordered_map< ControllerAxis, int >& serialConfig, ControllerMap& controllerMap )
  {
    const auto controllerSerial = controller->getSerialNumber();
    if( controllerSerial == 0 )
    {
      return;
    }

    for( const auto& [ axis, serial ] : serialConfig )
    {
      if( serial == controllerSerial )
      {
        controllerMap[ axis ] = controller;
        return;
      }
    }
  }

  void createControllerMap( const INIReader& reader, const std::unordered_map< ControllerAxis, int >& serialConfig,
    ControllerMap& controllerMap )
  {
    std::vector< std::string > comPorts = getAvailableComPorts( reader );
    ;
    for( const auto& port : comPorts )
    {
      moveControllerToMap( std::make_shared< LaingController >( port ), serialConfig, controllerMap );
    }
  }

  void createController( ControllerMap& controllerMap )
  {
    const auto reader = getReader( "cellulite.ini" );
    const auto serialConfig = readSerialConfig( reader );
    createControllerMap( reader, serialConfig, controllerMap );
  }

} // namespace

bool CelluliteApp::OnInit()
{
  ControllerMap controllerMap;
  try
  {
    createController( controllerMap );
  }
  catch( std::runtime_error& /*e*/ )
  {
    std::cout << "Failed to load configuration file: " << std::endl;
    exit( 1 );
  }

  auto* frame = new CelluliteFrame( controllerMap );
  frame->Show( true );
  return true;
}
