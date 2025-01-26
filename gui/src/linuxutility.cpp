#include "utility.h"

#include <iostream>

std::vector< std::string > getAvailableComPorts( const INIReader& reader )
{
  std::vector< std::string > comPorts;
  comPorts.emplace_back( getString( reader, "DEVICES", "one" ) );
  comPorts.emplace_back( getString( reader, "DEVICES", "two" ) );
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