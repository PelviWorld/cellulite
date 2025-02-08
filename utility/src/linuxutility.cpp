#include "utility.h"

#include <filesystem>
#include <iostream>

std::vector< std::string > getAvailableComPorts( const INIReader& /*reader*/ )
{
  std::vector< std::string > comPorts;

  for( const auto& entry : std::filesystem::directory_iterator( "/dev" ) )
  {
    if( entry.path().string().find( "ttyUSB" ) != std::string::npos )
    {
      comPorts.push_back( entry.path().string() );
    }
    if( entry.path().string().find( "ttyACM" ) != std::string::npos )
    {
      comPorts.push_back( entry.path().string() );
    }
  }
  return comPorts;
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