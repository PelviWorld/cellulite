#include "controllerframe.h"

#include <ranges>

wxDEFINE_EVENT( EVT_ENABLE_BUTTONS, wxCommandEvent );

constexpr int kBORDER = 5;
constexpr auto kPOLLING_INTERVAL = 100;

ControllerFrame::ControllerFrame( wxWindow* parent, const Controller& controller, const int idOffset )
  : wxPanel( parent )
  , m_controller( controller )
  , m_timer( this, wxID_ANY )
{
  createButtons( idOffset );

  m_tableHeightLabel = new wxStaticText( this, wxID_ANY, "Table Height:" );
  updateTableHeight();

  auto* frameSizer = new wxBoxSizer( wxVERTICAL );
  frameSizer->Add( m_tableHeightLabel, 0, wxALL, kBORDER );
  frameSizer->AddSpacer( 10 );
  frameSizer->Add( m_pos1Button, 0, wxALL, kBORDER );
  frameSizer->Add( m_pos2Button, 0, wxALL, kBORDER );
  frameSizer->AddSpacer( 50 );
  frameSizer->Add( m_moveUpButton, 0, wxALL, kBORDER );
  frameSizer->Add( m_moveDownButton, 0, wxALL, kBORDER );
  frameSizer->AddSpacer( 30 );
  frameSizer->Add( m_savePos1Button, 0, wxALL, kBORDER );
  frameSizer->Add( m_savePos2Button, 0, wxALL, kBORDER );
  frameSizer->AddStretchSpacer( 1 );
  frameSizer->Add( m_referenceButton, 0, wxALL, kBORDER );

  SetSizerAndFit( frameSizer );

  Bind( wxEVT_TIMER, &ControllerFrame::onTimer, this, m_timer.GetId() );
}

void ControllerFrame::createButtons( const int idOffset )
{
  m_pos1Button = new wxButton( this, wxID_ANY + idOffset, "Pos 1" );
  m_pos1Button->SetToolTip( "Move to position 1" );
  m_pos2Button = new wxButton( this, wxID_ANY + idOffset + 1, "Pos 2" );
  m_pos2Button->SetToolTip( "Move to position 2" );
  m_referenceButton = new wxButton( this, wxID_ANY + idOffset + 2, "Reference" );
  m_referenceButton->SetToolTip( "Reference run" );
  m_moveUpButton = new wxButton( this, wxID_ANY + idOffset + 3, "Move Up" );
  m_moveUpButton->SetToolTip( "Move up" );
  m_moveDownButton = new wxButton( this, wxID_ANY + idOffset + 4, "Move Down" );
  m_moveDownButton->SetToolTip( "Move down" );
  m_savePos1Button = new wxButton( this, wxID_ANY + idOffset + 5, "Save Pos 1" );
  m_savePos1Button->SetToolTip( "Save position 1" );
  m_savePos2Button = new wxButton( this, wxID_ANY + idOffset + 6, "Save Pos 2" );
  m_savePos2Button->SetToolTip( "Save position 2" );

  m_savePos1Button->Disable();
  m_savePos2Button->Disable();

  bindButtons( m_pos1Button );
  bindButtons( m_pos2Button );
  bindButtons( m_referenceButton );
  bindButtonsPressAndRelease( m_moveUpButton );
  bindButtonsPressAndRelease( m_moveDownButton );
  bindButtons( m_savePos1Button );
  bindButtons( m_savePos2Button );

  setBindingForReenablingButtons();
}

void ControllerFrame::bindButtons( wxButton* button )
{
  Bind( wxEVT_BUTTON, &ControllerFrame::onClicked, this, button->GetId() );
  bindHoverEvents( button );
}

void ControllerFrame::bindButtonsPressAndRelease( wxButton* button )
{
  button->Bind( wxEVT_LEFT_DOWN, &ControllerFrame::onButtonPress, this );
  button->Bind( wxEVT_LEFT_UP, &ControllerFrame::onButtonRelease, this );
  bindHoverEvents( button );
}

void ControllerFrame::bindHoverEvents( wxButton* button )
{
  button->Bind( wxEVT_ENTER_WINDOW, &ControllerFrame::onHoverEnter, this );
  button->Bind( wxEVT_LEAVE_WINDOW, &ControllerFrame::onHoverLeave, this );
}

void ControllerFrame::setBindingForReenablingButtons()
{
  if( m_controller != nullptr )
  {
    m_controller->setCallback( [ this ] { wxQueueEvent( this, new wxCommandEvent( EVT_ENABLE_BUTTONS ) ); } );
  }
  Bind( EVT_ENABLE_BUTTONS, &ControllerFrame::enableButtons, this );
}

void ControllerFrame::onHoverEnter( wxMouseEvent& event )
{
  if( const auto* button = dynamic_cast< wxButton* >( event.GetEventObject() ) )
  {
    if( auto* parentFrame = dynamic_cast< wxFrame* >( GetParent() ) )
    {
      parentFrame->SetStatusText( button->GetToolTipText() );
    }
  }
  event.Skip();
}

void ControllerFrame::onHoverLeave( wxMouseEvent& event )
{
  if( auto* parentFrame = dynamic_cast< wxFrame* >( GetParent() ) )
  {
    parentFrame->SetStatusText( "Cellulite APP" );
  }
  event.Skip();
}

void ControllerFrame::onClicked( wxCommandEvent& event )
{
  const int id = event.GetId();
  if( id == m_pos1Button->GetId() )
  {
    if( m_controller != nullptr )
    {
      disableButtons();
      m_controller->moveToUserPosition( AXIS::ONE, USER_POSITION::POS_1 );
    }
  }
  else if( id == m_pos2Button->GetId() )
  {
    if( m_controller != nullptr )
    {
      disableButtons();
      m_controller->moveToUserPosition( AXIS::ONE, USER_POSITION::POS_2 );
    }
  }
  else if( id == m_referenceButton->GetId() )
  {
    if( m_controller != nullptr )
    {
      disableButtons();
      m_controller->referenceRun( AXIS::ONE );
    }
  }
  else if( id == m_savePos1Button->GetId() )
  {
    if( m_controller != nullptr )
    {
      m_controller->saveUserPosition( AXIS::ONE, USER_POSITION::POS_1 );
    }
  }
  else if( id == m_savePos2Button->GetId() )
  {
    if( m_controller != nullptr )
    {
      m_controller->saveUserPosition( AXIS::ONE, USER_POSITION::POS_2 );
    }
  }
}

void ControllerFrame::onButtonPress( wxMouseEvent& event )
{
  const int id = event.GetId();
  if( id == m_moveUpButton->GetId() )
  {
    if( m_controller != nullptr )
    {
      m_controller->setMoveUpDown( AXIS::ONE, USER_POSITION::MOVE_UP );
      disableButtons();
      m_timer.Start( kPOLLING_INTERVAL );
    }
  }
  else if( id == m_moveDownButton->GetId() )
  {
    if( m_controller != nullptr )
    {
      m_controller->setMoveUpDown( AXIS::ONE, USER_POSITION::MOVE_DOWN );
      disableButtons();
      m_timer.Start( kPOLLING_INTERVAL );
    }
  }
  event.Skip();
}

void ControllerFrame::onButtonRelease( wxMouseEvent& event )
{
  const int id = event.GetId();
  if( id == m_moveUpButton->GetId() )
  {
    if( m_controller != nullptr )
    {
      m_controller->setUpDownDisabled( AXIS::ONE );
      enableButtons();
    }
  }
  else if( id == m_moveDownButton->GetId() )
  {
    if( m_controller != nullptr )
    {
      m_controller->setUpDownDisabled( AXIS::ONE );
      enableButtons();
    }
  }
  event.Skip();
}

void ControllerFrame::enableButtons()
{
  m_pos1Button->Enable();
  m_pos2Button->Enable();
  m_referenceButton->Enable();
  m_moveUpButton->Enable();
  m_moveDownButton->Enable();
  m_timer.Stop();
  updateTableHeight();
}

void ControllerFrame::enableButtons( wxCommandEvent& /*event*/ )
{
  enableButtons();
}

void ControllerFrame::disableButtons()
{
  m_pos1Button->Disable();
  m_pos2Button->Disable();
  m_referenceButton->Disable();
  m_moveUpButton->Disable();
  m_moveDownButton->Disable();
}

void ControllerFrame::onTimer( wxTimerEvent& /*event*/ )
{
  updateTableHeight();
}

void ControllerFrame::updateTableHeight()
{
  if( m_controller != nullptr )
  {
    const auto height = m_controller->getTableHeight( AXIS::ONE );
    m_tableHeightLabel->SetLabel( wxString::Format( "Table Height: %d", height ) );
  }
}
void ControllerFrame::updateSaveButtons( const int value )
{
  const auto button1 = ( ( 47 <= value ) && ( value <= 53 ) );
  const auto button2 = ( ( 82 <= value ) && ( value <= 88 ) );

  if( button1 )
  {
    m_savePos1Button->Enable();
  }
  else
  {
    m_savePos1Button->Disable();
  }

  if( button2 )
  {
    m_savePos2Button->Enable();
  }
  else
  {
    m_savePos2Button->Disable();
  }
}