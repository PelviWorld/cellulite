#include "celluliteapp.h"
#include "celluliteframe.h"

bool CelluliteApp::OnInit()
{
  auto* frame = new CelluliteFrame();
  frame->Show( true );
  return true;
}
