#include "celluliteapp.h"
#include "celluliteframe.h"

bool CelluliteApp::OnInit()
{
  auto* frame = new CelluliteFrame();
  frame->Show( true );
  return true;
}
LaingController& CelluliteApp::getController( const ControllerAxis axis )
{
  assert( m_controller.contains( axis ) );
  return m_controller[ axis ];
}
