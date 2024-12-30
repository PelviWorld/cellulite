#include "celluliteframe.h"
#include "cg_version.h"

#include <ranges>

namespace
{
  constexpr auto kWINDOW_WIDTH = 480;
  constexpr auto kWINDOW_HEIGHT = 800;
  constexpr auto kWIDGET_BORDER = 15;
}

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
  auto* pos1Button = new wxButton( this, ID_POS_1, "POSITION 1" );
  auto* pos2Button = new wxButton( this, ID_POS_2, "POSITION 2" );
  auto* referenceButton = new wxButton( this, ID_REFERENCE, "START REFERENCE RUN" );

  const auto bgColour = trainingButton->GetBackgroundColour();
  wxWindowBase::SetBackgroundColour( bgColour );

  auto* frameSizer = new wxBoxSizer( wxVERTICAL );
  frameSizer->Add( pos1Button, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, kWIDGET_BORDER );
  frameSizer->Add( pos2Button, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, kWIDGET_BORDER );
  frameSizer->AddStretchSpacer( 1 );
  frameSizer->Add( referenceButton, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, kWIDGET_BORDER );
  frameSizer->Add( trainingButton, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, kWIDGET_BORDER );
  SetSizer( frameSizer );

  Bind( wxEVT_BUTTON, &CelluliteFrame::onPos1, this, ID_POS_1 );
  Bind( wxEVT_BUTTON, &CelluliteFrame::onPos2, this, ID_POS_2 );
  Bind( wxEVT_BUTTON, &CelluliteFrame::onReference, this, ID_REFERENCE );
  Bind( wxEVT_BUTTON, &CelluliteFrame::onTraining, this, ID_TRAINING );
}

CelluliteFrame::CelluliteFrame( const ControllerMap& controllerMap )
  : wxFrame( nullptr, wxID_ANY, "Cellulite", wxDefaultPosition, wxSize( kWINDOW_WIDTH, kWINDOW_HEIGHT ),
      wxDEFAULT_FRAME_STYLE & ~( wxRESIZE_BORDER | wxMAXIMIZE_BOX ) )
  , m_controllerMap( controllerMap )
{
  wxTopLevelWindowBase::SetMinSize( wxSize( kWINDOW_WIDTH, kWINDOW_HEIGHT ) );
  wxTopLevelWindowBase::SetMaxSize( wxSize( kWINDOW_WIDTH, kWINDOW_HEIGHT ) );
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
  wxMessageBox( "This is the Cellulite App Version\n" + std::string( CG_VERSION ), "About Cellulite App",
    wxOK | wxICON_INFORMATION );
}
void CelluliteFrame::onTraining( wxCommandEvent& event )
{
  wxLogMessage( "Training started!" );
}

void CelluliteFrame::onHello( wxCommandEvent& event )
{
  wxLogMessage( "Hello from your Cellulite App!" );
}

void CelluliteFrame::onPos1( wxCommandEvent& event )
{
  wxLogMessage( "POSITION 1 button clicked!" );
  for( const auto controller : m_controllerMap | std::views::values )
  {
    controller->moveToUserPosition( AXIS::ONE, USER_POSITION::POS_1 );
  }
}

void CelluliteFrame::onPos2( wxCommandEvent& event )
{
  wxLogMessage( "POSITIPN 2 button clicked!" );
  for( const auto controller : m_controllerMap | std::views::values )
  {
    controller->moveToUserPosition( AXIS::ONE, USER_POSITION::POS_2 );
  }
}

void CelluliteFrame::onReference( wxCommandEvent& event )
{
  wxLogMessage( "REFERENCE button clicked!" );
  for( const auto controller : m_controllerMap | std::views::values )
  {
    controller->referenceRun( AXIS::ONE );
  }
}