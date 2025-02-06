#include "celluliteapp.h"
#include "configreader.h"

bool CelluliteApp::OnInit()
{
  ControllerMap controllerMap;
  auto remainingPorts = createControllerMapAndReturnRemainingPorts( controllerMap );
  m_gyroCom = createGyro( remainingPorts );
  m_frame = std::make_unique< CelluliteFrame >( controllerMap );

  m_frame->Show( true );
  SetTopWindow( m_frame.get() );

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
  if( m_gyroCom != nullptr )
  {
    m_pitch = m_gyroCom->readDouble();
    m_frame->setPitch( m_pitch );
  }
}

void CelluliteApp::readLoop()
{
  constexpr auto kREAD_INTERVAL = 150;
  while( running )
  {
    readDataFromPico();
    std::this_thread::sleep_for( std::chrono::milliseconds( kREAD_INTERVAL ) );
  }
}