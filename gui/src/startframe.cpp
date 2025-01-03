#include "startframe.h"

#include <ranges>

wxDEFINE_EVENT( EVT_ENABLE_BUTTONS, wxCommandEvent );

StartFrame::StartFrame( wxWindow* parent, const Controller& controller, int idOffset )
  : wxPanel( parent )
  , m_controller( controller )
{
  m_pos1Button = new wxButton( this, wxID_ANY + idOffset, "Pos 1" );
  m_pos2Button = new wxButton( this, wxID_ANY + idOffset + 1, "Pos 2" );
  m_referenceButton = new wxButton( this, wxID_ANY + idOffset + 2, "Reference" );

  Bind( wxEVT_BUTTON, &StartFrame::onPos1, this, m_pos1Button->GetId() );
  Bind( wxEVT_BUTTON, &StartFrame::onPos2, this, m_pos2Button->GetId() );
  Bind( wxEVT_BUTTON, &StartFrame::onReference, this, m_referenceButton->GetId() );

  m_pos1Button->Bind( wxEVT_ENTER_WINDOW, &StartFrame::onHoverEnter, this );
  m_pos1Button->Bind( wxEVT_LEAVE_WINDOW, &StartFrame::onHoverLeave, this );
  m_pos2Button->Bind( wxEVT_ENTER_WINDOW, &StartFrame::onHoverEnter, this );
  m_pos2Button->Bind( wxEVT_LEAVE_WINDOW, &StartFrame::onHoverLeave, this );
  m_referenceButton->Bind( wxEVT_ENTER_WINDOW, &StartFrame::onHoverEnter, this );
  m_referenceButton->Bind( wxEVT_LEAVE_WINDOW, &StartFrame::onHoverLeave, this );

  if( m_controller != nullptr )
  {
    m_controller->setCallback( [ this ] { wxQueueEvent( this, new wxCommandEvent( EVT_ENABLE_BUTTONS ) ); } );
  }
  Bind( EVT_ENABLE_BUTTONS, &StartFrame::enableButtons, this );

  auto* frameSizer = new wxBoxSizer( wxVERTICAL );
  frameSizer->Add( m_pos1Button, 0, wxALL, 5 );
  frameSizer->Add( m_pos2Button, 0, wxALL, 5 );
  frameSizer->AddStretchSpacer( 1 );
  frameSizer->Add( m_referenceButton, 0, wxALL, 5 );

  SetSizerAndFit( frameSizer );
}

void StartFrame::onHoverEnter( wxMouseEvent& event )
{
  if( auto* button = dynamic_cast< wxButton* >( event.GetEventObject() ) )
  {
    button->SetBackgroundColour( *wxLIGHT_GREY );
    if( auto* parentFrame = dynamic_cast< wxFrame* >( GetParent() ) )
    {
      parentFrame->SetStatusText( button->GetToolTipText() );
    }
  }
  event.Skip();
}

void StartFrame::onHoverLeave( wxMouseEvent& event )
{
  if( auto* button = dynamic_cast< wxButton* >( event.GetEventObject() ) )
  {
    button->SetBackgroundColour( wxNullColour );
    if( auto* parentFrame = dynamic_cast< wxFrame* >( GetParent() ) )
    {
      parentFrame->SetStatusText( "Cellulite APP" );
    }
  }
  event.Skip();
}

void StartFrame::onPos1( wxCommandEvent& event )
{
  if( m_controller != nullptr )
  {
    disableButtons();
    m_controller->moveToUserPosition( AXIS::ONE, USER_POSITION::POS_1 );
  }
}

void StartFrame::onPos2( wxCommandEvent& event )
{
  if( m_controller != nullptr )
  {
    disableButtons();
    m_controller->moveToUserPosition( AXIS::ONE, USER_POSITION::POS_2 );
  }
}

void StartFrame::onReference( wxCommandEvent& event )
{
  if( m_controller != nullptr )
  {
    disableButtons();
    m_controller->referenceRun( AXIS::ONE );
  }
}

void StartFrame::onTraining( wxCommandEvent& event )
{
  // Implement the training functionality here
}

void StartFrame::enableButtons( wxCommandEvent& event )
{
  m_pos1Button->Enable();
  m_pos2Button->Enable();
  m_referenceButton->Enable();
}

void StartFrame::disableButtons()
{
  m_pos1Button->Disable();
  m_pos2Button->Disable();
  m_referenceButton->Disable();
}