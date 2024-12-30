#include "celluliteapp.h"
#include <wx/wx.h>

bool CelluliteApp::OnInit()
{
  CelluliteFrame* frame = new CelluliteFrame();
  frame->Show( true );
  return true;
}

CelluliteFrame::CelluliteFrame()
  : wxFrame( nullptr, wxID_ANY, "Cellulite" )
{
  wxMenu* menuFile = new wxMenu;
  menuFile->Append( ID_Hello, "&Hello...\tCtrl-H", "Greetings from Cellulite App" );
  menuFile->AppendSeparator();
  menuFile->Append( wxID_EXIT );

  wxMenu* menuHelp = new wxMenu;
  menuHelp->Append( wxID_ABOUT );

  wxMenuBar* menuBar = new wxMenuBar;
  menuBar->Append( menuFile, "&File" );
  menuBar->Append( menuHelp, "&Help" );

  SetMenuBar( menuBar );

  CreateStatusBar();
  SetStatusText( "Welcome to Cellulite App!" );

  Bind( wxEVT_MENU, &CelluliteFrame::onHello, this, ID_Hello );
  Bind( wxEVT_MENU, &CelluliteFrame::onAbout, this, wxID_ABOUT );
  Bind( wxEVT_MENU, &CelluliteFrame::onExit, this, wxID_EXIT );
}

void CelluliteFrame::onExit( wxCommandEvent& event )
{
  Close( true );
}

void CelluliteFrame::onAbout( wxCommandEvent& event )
{
  wxMessageBox( "This is the Cellulite App", "About Cellulite App", wxOK | wxICON_INFORMATION );
}

void CelluliteFrame::onHello( wxCommandEvent& event )
{
  wxLogMessage( "Hello from your Cellulite App!" );
}