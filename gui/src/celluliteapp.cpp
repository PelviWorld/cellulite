#include "celluliteapp.h"
#include "INIReader.h"
#include "utility.h"

#include <array>
#include <regex>

namespace
{
  const std::string tilt = "Seat tilt: ";

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

  std::vector< std::string > createControllerMap( const INIReader& reader,
    const std::unordered_map< ControllerAxis, int >& serialConfig, ControllerMap& controllerMap )
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

} // namespace

bool CelluliteApp::OnInit()
{
  ControllerMap controllerMap;
  std::vector< std::string > ports;
  try
  {
    ports = createController( controllerMap );
  }
  catch( std::runtime_error& /*e*/ )
  {
    std::cout << "Failed to load configuration file: " << std::endl;
    exit( 1 );
  }

  for( auto port : ports )
  {
    m_gyroCom = std::make_unique< GyroCom >( port );
    if( m_gyroCom->verifyConnection() )
    {
      break;
    }
    m_gyroCom.reset();
  }

  m_frame = std::make_unique< CelluliteFrame >( controllerMap );
  m_frame->Show( true );
  SetTopWindow( m_frame.get() );

  m_actualPitchLabel = new wxStaticText( m_frame.get(), wxID_ANY, tilt, wxPoint( 150, 5 ) );
  readThread = std::thread( &CelluliteApp::readLoop, this );

  return true;
}
int CelluliteApp::OnExit()
{
  running = false;
  if( readThread.joinable() )
  {
    readThread.join();
  }
  wxSleep( 1 );
  return wxApp::OnExit();
}

void CelluliteApp::readDataFromPico()
{
  if( m_gyroCom == nullptr )
  {
    m_actualPitchLabel->SetLabel( "No gyro" );
    return;
  }
  try
  {
    const std::string data = m_gyroCom->readData();
    if( data.empty() || data[ 0 ] != 'P' )
    {
      return;
    }
    std::smatch match;
    const std::regex numberRegex(
      R"(Pitch:\s*(-?\d+))" ); // Regular expression to match a number at the end of the string

    if( std::regex_search( data, match, numberRegex ) )
    {
      const std::string number = match.str( 1 ); // Extract the matched number
      if( m_actualPitchLabel != nullptr )
      {
        m_actualPitchLabel->SetLabel( tilt + number );
      }
      m_frame->updateSaveButtons( std::abs( std::stoi( number ) ) );
    }
  }
  catch( const std::exception& e )
  {
    std::cerr << "Exception in readDataFromPico: " << e.what() << std::endl;
  }
}
void CelluliteApp::readLoop()
{
  while( running )
  {
    readDataFromPico();
    std::this_thread::sleep_for( std::chrono::milliseconds( 150 ) );
  }
}