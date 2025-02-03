#include "ConfigReader.h"
#include "utility.h"

#include <iostream>

int getValue( const INIReader& reader, const std::string& section, const std::string& key )
{
  const int value = reader.GetInteger( section, key, -1 );
  if( value == -1 )
  {
    std::cerr << "Can't find " << key << " in " << section << " section\n" << std::endl;
    throw std::runtime_error( "Can't find " + key + " in " + section + " section" );
  }
  return value;
}

INIReader getReader( const std::string& filename )
{
  INIReader reader( filename );
  if( reader.ParseError() != 0 )
  {
    std::cerr << "Can't load 'cellulite.ini'\n" << std::endl;
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
    std::cerr << "Serial numbers are the same" << std::endl;
    throw std::runtime_error( "Serial numbers are the same" );
  }
  return result;
}

bool moveControllerToMap( const std::shared_ptr< LaingController >& controller,
  const std::unordered_map< ControllerAxis, int >& serialConfig, ControllerMap& controllerMap )
{
  const auto controllerSerial = controller->getSerialNumber();
  if( controllerSerial == 0 )
  {
    return false;
  }

  for( const auto& [ axis, serial ] : serialConfig )
  {
    if( serial == controllerSerial )
    {
      controllerMap[ axis ] = controller;
      return true;
    }
  }
  return false;
}

std::vector< std::string > createControllerMap(
  const INIReader& reader, const std::unordered_map< ControllerAxis, int >& serialConfig, ControllerMap& controllerMap )
{
  const std::vector< std::string > comPorts = getAvailableComPorts( reader );
  std::vector< std::string > remainingPorts;
  for( const auto& port : comPorts )
  {
    if( moveControllerToMap( std::make_shared< LaingController >( port ), serialConfig, controllerMap ) == false )
    {
      remainingPorts.push_back( port );
    }
  }
  return remainingPorts;
}

std::vector< std::string > createController( ControllerMap& controllerMap )
{
  const auto reader = getReader( "cellulite.ini" );
  const auto serialConfig = readSerialConfig( reader );
  return createControllerMap( reader, serialConfig, controllerMap );
}
