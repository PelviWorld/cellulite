#include "controllerframe.h"

#include <ranges>

wxDEFINE_EVENT( EVT_ENABLE_BUTTONS, wxCommandEvent );

constexpr int kBORDER = 5;

ControllerFrame::ControllerFrame( wxWindow* parent, const Controller& controller, const int idOffset )
  : wxPanel( parent )
  , m_controller( controller )
{
  createButtons( idOffset );

  m_tableHeightLabel = new wxStaticText( this, wxID_ANY, "Table Height:" );
  updateTableHeight();

  auto* frameSizer = new wxBoxSizer( wxVERTICAL );
  frameSizer->Add( m_tableHeightLabel, 0, wxALL, kBORDER );
  frameSizer->Add( m_pos1Button, 0, wxALL, kBORDER );
  frameSizer->Add( m_pos2Button, 0, wxALL, kBORDER );
  frameSizer->AddStretchSpacer( 1 );
  frameSizer->Add( m_referenceButton, 0, wxALL, kBORDER );

  SetSizerAndFit( frameSizer );
}

void ControllerFrame::createButtons( const int idOffset )
{
  m_pos1Button = new wxButton( this, wxID_ANY + idOffset, "Pos 1" );
  m_pos1Button->SetToolTip( "Move to position 1" );
  m_pos2Button = new wxButton( this, wxID_ANY + idOffset + 1, "Pos 2" );
  m_pos2Button->SetToolTip( "Move to position 2" );
  m_referenceButton = new wxButton( this, wxID_ANY + idOffset + 2, "Reference" );
  m_referenceButton->SetToolTip( "Reference run" );

  bindButtons( m_pos1Button );
  bindButtons( m_pos2Button );
  bindButtons( m_referenceButton );

  setBindingForReenablingButtons();
}

void ControllerFrame::bindButtons( wxButton* button )
{
  button->Bind( wxEVT_ENTER_WINDOW, &ControllerFrame::onHoverEnter, this );
  button->Bind( wxEVT_LEAVE_WINDOW, &ControllerFrame::onHoverLeave, this );
  Bind( wxEVT_BUTTON, &ControllerFrame::onPos1, this, button->GetId() );
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

void ControllerFrame::onPos1( wxCommandEvent& event )
{
  if( m_controller != nullptr )
  {
    disableButtons();
    m_controller->moveToUserPosition( AXIS::ONE, USER_POSITION::POS_1 );
  }
}

void ControllerFrame::onPos2( wxCommandEvent& event )
{
  if( m_controller != nullptr )
  {
    disableButtons();
    m_controller->moveToUserPosition( AXIS::ONE, USER_POSITION::POS_2 );
  }
}

void ControllerFrame::onReference( wxCommandEvent& event )
{
  if( m_controller != nullptr )
  {
    disableButtons();
    m_controller->referenceRun( AXIS::ONE );
  }
}

void ControllerFrame::enableButtons( wxCommandEvent& event )
{
  m_pos1Button->Enable();
  m_pos2Button->Enable();
  m_referenceButton->Enable();
  updateTableHeight();
}

void ControllerFrame::disableButtons()
{
  m_pos1Button->Disable();
  m_pos2Button->Disable();
  m_referenceButton->Disable();
}
void ControllerFrame::updateTableHeight()
{
  if( m_controller != nullptr )
  {
    const auto height = m_controller->getTableHeight( AXIS::ONE );
    m_tableHeightLabel->SetLabel( wxString::Format( "Table Height: %d", height ) );
  }
}