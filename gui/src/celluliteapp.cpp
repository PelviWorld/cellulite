#include "celluliteapp.h"
#include "configreader.h"

#include <regex>

const std::string tilt = "Seat tilt: ";

bool CelluliteApp::OnInit()
{
  ControllerMap controllerMap;
  auto remainingPorts = createControllerMapAndReturnRemainingPorts( controllerMap );
  m_gyroCom = createGyro( remainingPorts );
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