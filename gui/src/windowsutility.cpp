#include "windowsutility.h"

#include <array>
#include <iostream>
#include <windows.h>

std::vector< std::string > getAvailableComPorts( const INIReader& /*reader*/ )
{
  constexpr auto kSZ_DEVICES = 65535;
  std::vector< std::string > comPorts;
  std::array< char, kSZ_DEVICES > szDevices{};
  const DWORD dwRet = QueryDosDeviceA( nullptr, szDevices.data(), kSZ_DEVICES );
  if( dwRet != 0U )
  {
    const char* pszNext = szDevices.data();
    while( *pszNext != 0 )
    {
      if( strncmp( pszNext, "COM", 3 ) == 0 )
      {
        comPorts.push_back( pszNext );
      }
      pszNext += strlen( pszNext ) + 1; // NOLINT(*-pro-bounds-pointer-arithmetic)
    }
  }
  else
  {
    std::cerr << "QueryDosDeviceA failed with error: " << GetLastError() << std::endl;
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
