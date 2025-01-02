#include "celluliteframe.h"
#include "cg_version.h"

#include <ranges>

wxDEFINE_EVENT( EVT_ENABLE_BUTTONS, wxCommandEvent );

namespace
{
  constexpr auto kWINDOW_WIDTH = 480;
  constexpr auto kWINDOW_HEIGHT = 800;
  constexpr auto kWIDGET_BORDER = 15;
  const std::string kAPP_NAME = "Cellulite APP";
}

void CelluliteFrame::createMenuBar()
{
  auto* menuFile = new wxMenu;
  menuFile->Append( ID_HELLO, "&Hello...\tCtrl-H", "Greetings from " + kAPP_NAME );
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
  wxFrameBase::SetStatusText( kAPP_NAME );
}

void CelluliteFrame::createStartFrame()
{
  auto* trainingButton = new wxButton( this, ID_TRAINING, "Start Training" );
  trainingButton->SetToolTip( "Start the trainings session" );
  m_pos1Button = new wxButton( this, ID_POS_1, "POSITION 1" );
  m_pos1Button->SetToolTip( "Move to Position 1" );
  m_pos2Button = new wxButton( this, ID_POS_2, "POSITION 2" );
  m_pos2Button->SetToolTip( "Move to Position 2" );
  m_referenceButton = new wxButton( this, ID_REFERENCE, "START REFERENCE RUN" );
  m_referenceButton->SetToolTip( "Start the reference run" );

  const auto bgColour = trainingButton->GetBackgroundColour();
  wxWindowBase::SetBackgroundColour( bgColour );

  auto* frameSizer = new wxBoxSizer( wxVERTICAL );
  frameSizer->Add( m_pos1Button, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, kWIDGET_BORDER );
  frameSizer->Add( m_pos2Button, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, kWIDGET_BORDER );
  frameSizer->AddStretchSpacer( 1 );
  frameSizer->Add( m_referenceButton, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, kWIDGET_BORDER );
  frameSizer->Add( trainingButton, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, kWIDGET_BORDER );
  SetSizer( frameSizer );

  Bind( wxEVT_BUTTON, &CelluliteFrame::onPos1, this, ID_POS_1 );
  Bind( wxEVT_BUTTON, &CelluliteFrame::onPos2, this, ID_POS_2 );
  Bind( wxEVT_BUTTON, &CelluliteFrame::onReference, this, ID_REFERENCE );
  Bind( wxEVT_BUTTON, &CelluliteFrame::onTraining, this, ID_TRAINING );

  m_pos1Button->Bind( wxEVT_ENTER_WINDOW, &CelluliteFrame::onHoverEnter, this );
  m_pos1Button->Bind( wxEVT_LEAVE_WINDOW, &CelluliteFrame::onHoverLeave, this );
  m_pos2Button->Bind( wxEVT_ENTER_WINDOW, &CelluliteFrame::onHoverEnter, this );
  m_pos2Button->Bind( wxEVT_LEAVE_WINDOW, &CelluliteFrame::onHoverLeave, this );
  m_referenceButton->Bind( wxEVT_ENTER_WINDOW, &CelluliteFrame::onHoverEnter, this );
  m_referenceButton->Bind( wxEVT_LEAVE_WINDOW, &CelluliteFrame::onHoverLeave, this );
  trainingButton->Bind( wxEVT_ENTER_WINDOW, &CelluliteFrame::onHoverEnter, this );
  trainingButton->Bind( wxEVT_LEAVE_WINDOW, &CelluliteFrame::onHoverLeave, this );
}

CelluliteFrame::CelluliteFrame( const ControllerMap& controllerMap )
  : wxFrame( nullptr, wxID_ANY, kAPP_NAME, wxDefaultPosition, wxSize( kWINDOW_WIDTH, kWINDOW_HEIGHT ),
      wxDEFAULT_FRAME_STYLE & ~( wxRESIZE_BORDER | wxMAXIMIZE_BOX ) )
  , m_controllerMap( controllerMap )
{
  wxTopLevelWindowBase::SetMinSize( wxSize( kWINDOW_WIDTH, kWINDOW_HEIGHT ) );
  wxTopLevelWindowBase::SetMaxSize( wxSize( kWINDOW_WIDTH, kWINDOW_HEIGHT ) );
  createMenuBar();
  crateStatusBar();
  createStartFrame();

  for( const auto& controller : m_controllerMap | std::views::values )
  {
    controller->setCallback( [ this ] { wxQueueEvent( this, new wxCommandEvent( EVT_ENABLE_BUTTONS ) ); } );
  }

  Bind( EVT_ENABLE_BUTTONS, &CelluliteFrame::enableButtons, this );
}

void CelluliteFrame::disableButtons()
{
  m_pos1Button->Disable();
  m_pos2Button->Disable();
  m_referenceButton->Disable();
};

void CelluliteFrame::enableButtons( wxCommandEvent& event )
{
  m_pos1Button->Enable();
  m_pos2Button->Enable();
  m_referenceButton->Enable();
}

void CelluliteFrame::onHoverEnter( wxMouseEvent& event )
{
  if( auto* button = dynamic_cast< wxButton* >( event.GetEventObject() ) )
  {
    button->SetBackgroundColour( *wxLIGHT_GREY );
    SetStatusText( button->GetToolTipText() );
  }
  event.Skip();
}

void CelluliteFrame::onHoverLeave( wxMouseEvent& event )
{
  if( auto* button = dynamic_cast< wxButton* >( event.GetEventObject() ) )
  {
    button->SetBackgroundColour( wxNullColour );
    SetStatusText( kAPP_NAME );
  }
  event.Skip();
}

void CelluliteFrame::onExit( wxCommandEvent& event )
{
  Close( true );
}

void CelluliteFrame::onAbout( wxCommandEvent& event )
{
  wxMessageBox( "This is the " + kAPP_NAME + " Version\n" + std::string( CG_VERSION ), "About Cellulite App",
    wxOK | wxICON_INFORMATION );
}
void CelluliteFrame::onTraining( wxCommandEvent& event )
{
  wxLogMessage( "Training started!" );
}

void CelluliteFrame::onHello( wxCommandEvent& event )
{
  wxLogMessage( ( "Hello from your " + kAPP_NAME + "!" ).c_str() );
}

void CelluliteFrame::onPos1( wxCommandEvent& event )
{
  disableButtons();
  for( const auto controller : m_controllerMap | std::views::values )
  {
    controller->moveToUserPosition( AXIS::ONE, USER_POSITION::POS_1 );
  }
}

void CelluliteFrame::onPos2( wxCommandEvent& event )
{
  disableButtons();
  for( const auto controller : m_controllerMap | std::views::values )
  {
    controller->moveToUserPosition( AXIS::ONE, USER_POSITION::POS_2 );
  }
}

void CelluliteFrame::onReference( wxCommandEvent& event )
{
  disableButtons();
  for( const auto controller : m_controllerMap | std::views::values )
  {
    controller->referenceRun( AXIS::ONE );
  }
}