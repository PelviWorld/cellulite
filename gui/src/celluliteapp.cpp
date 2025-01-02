#include "celluliteapp.h"
#include "INIReader.h"
#include "celluliteframe.h"

#include <array>

#ifdef WIN32
#include <windows.h>
namespace
{
  std::vector<std::string> getAvailableComPorts()
  {
    constexpr auto kSZ_DEVICES = 65535;
    std::vector<std::string> comPorts;
    std::array<char,kSZ_DEVICES> szDevices{};
    const DWORD dwRet = QueryDosDeviceA( nullptr, szDevices.data(), kSZ_DEVICES  );
    if (dwRet != 0U)
    {
      const char* pszNext = szDevices.data();
      while (*pszNext != 0)
      {
        if (strncmp(pszNext, "COM", 3) == 0)
        {
          comPorts.push_back(pszNext);
        }
        pszNext += strlen(pszNext) + 1; // NOLINT(*-pro-bounds-pointer-arithmetic)
      }
    }
    else
    {
      std::cerr << "QueryDosDeviceA failed with error: " << GetLastError() << std::endl;
    }
    return comPorts;
  }
}
#endif

namespace
{
#ifndef WIN32
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
#endif

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
#ifdef WIN32
    for ( const std::vector< std::string > comPorts = getAvailableComPorts(); const auto& port : comPorts)
    {
      moveControllerToMap( std::make_shared< LaingController >( port ), serialConfig, controllerMap );
    }
#else
    std::string deviceOne = getString( reader, "DEVICES", "one" );
    std::string deviceTwo = getString( reader, "DEVICES", "two" );
    moveControllerToMap( std::make_shared< LaingController >( deviceOne ), serialConfig, controllerMap );
    moveControllerToMap( std::make_shared< LaingController >( deviceTwo ), serialConfig, controllerMap );
#endif

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
