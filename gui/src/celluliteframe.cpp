#include "celluliteframe.h"

void CelluliteFrame::createMenuBar()
{
  auto* menuFile = new wxMenu;
  menuFile->Append( ID_HELLO, "&Hello...\tCtrl-H", "Greetings from Cellulite App" );
  menuFile->AppendSeparator();
  menuFile->Append( wxID_EXIT );

  auto* menuHelp = new wxMenu;
  menuHelp->Append( wxID_ABOUT );

  auto* menuBar = new wxMenuBar;
  menuBar->Append( menuFile, "&File" );
  menuBar->Append( menuHelp, "&Help" );

  wxFrameBase::SetMenuBar( menuBar );

  Bind( wxEVT_MENU, &CelluliteFrame::onHello, this, ID_HELLO );
  Bind( wxEVT_MENU, &CelluliteFrame::onAbout, this, wxID_ABOUT );
  Bind( wxEVT_MENU, &CelluliteFrame::onExit, this, wxID_EXIT );
}

void CelluliteFrame::crateStatusBar()
{
  wxFrameBase::CreateStatusBar();
  wxFrameBase::SetStatusText( "Welcome to Cellulite App!" );
}

void CelluliteFrame::createStartFrame()
{
  auto* trainingButton = new wxButton( this, ID_TRAINING, "Start Training" );
  const auto bgColour = trainingButton->GetBackgroundColour();
  wxWindowBase::SetBackgroundColour( bgColour );

  auto* frameSizer = new wxBoxSizer( wxVERTICAL );
  frameSizer->AddStretchSpacer( 1 );
  frameSizer->Add( trainingButton, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 15 );
  SetSizer( frameSizer );

  Bind( wxEVT_BUTTON, &CelluliteFrame::onTraining, this, ID_TRAINING );
}

CelluliteFrame::CelluliteFrame()
  : wxFrame( nullptr, wxID_ANY, "Cellulite" )
{
  createMenuBar();
  crateStatusBar();
  createStartFrame();
}

void CelluliteFrame::onExit( wxCommandEvent& event )
{
  Close( true );
}

void CelluliteFrame::onAbout( wxCommandEvent& event )
{
  wxMessageBox( "This is the Cellulite App", "About Cellulite App", wxOK | wxICON_INFORMATION );
}
void CelluliteFrame::onTraining( wxCommandEvent& event )
{
  wxLogMessage( "Training started!" );
}

void CelluliteFrame::onHello( wxCommandEvent& event )
{
  wxLogMessage( "Hello from your Cellulite App!" );
}