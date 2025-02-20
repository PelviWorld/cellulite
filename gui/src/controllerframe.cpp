#include "controllerframe.h"

#include <wx/dcbuffer.h>

wxDEFINE_EVENT( EVT_ENABLE_BUTTONS, wxCommandEvent );

namespace
{
  constexpr auto kBORDER = 5;
  constexpr auto kPOLLING_INTERVAL = 100;
  constexpr auto kSMALL_SPACER = 10;
  constexpr auto kBIG_SPACER = 50;
  constexpr auto kBUTTON_SPACER = 30;
  const wxPoint kCENTER_SEAT( 243, 235 );
  constexpr auto kHALF_CIRCLE = 180;
  const auto kWHITE_BG = wxColour( 255, 255, 255 );
  constexpr auto kPICTURE_ANGLE_OFFSET = -10;

  double degreesToRadians( double degrees )
  {
    return degrees * M_PI / kHALF_CIRCLE;
  }

  wxImage rotateImage( wxImage* image, double angle )
  {
    return image->Rotate( degreesToRadians( angle ), wxPoint( image->GetWidth() / 2, image->GetHeight() / 2 ) );
  }

  wxImage* createImage( wxWindow* parent, std::string imageName, bool useAlpha = true )
  {
    auto* image = new wxImage( imageName, wxBITMAP_TYPE_PNG );
    if( image->IsOk() && useAlpha )
    {
      image->SetMaskColour( kWHITE_BG.Red(), kWHITE_BG.Green(), kWHITE_BG.Blue() );
      image->InitAlpha();
    }
    return image;
  }

  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
  wxBitmap* combineBitmaps( wxImage* bgImage, wxImage* seatImage, const wxPoint& centerOfSeatImage = kCENTER_SEAT )
  {
    wxBitmap combinedBitmap( bgImage->GetWidth(), bgImage->GetHeight() );
    wxMemoryDC memDC( combinedBitmap );

    auto rotatedSeatImage = rotateImage( seatImage, kPICTURE_ANGLE_OFFSET );
    memDC.SetBrush( *wxWHITE_BRUSH );
    memDC.SetPen( *wxWHITE_PEN );
    memDC.DrawRectangle( 0, 0, bgImage->GetWidth(), bgImage->GetHeight() );
    memDC.DrawBitmap( wxBitmap( *bgImage ), 0, 0, true );
    memDC.DrawBitmap( wxBitmap( rotatedSeatImage ), centerOfSeatImage.x - ( rotatedSeatImage.GetWidth() / 2 ),
      centerOfSeatImage.y - ( rotatedSeatImage.GetHeight() / 2 ), true );
    memDC.SelectObject( wxNullBitmap );

    wxImage combinedImage = combinedBitmap.ConvertToImage();
    combinedImage.SetMaskColour( kWHITE_BG.Red(), kWHITE_BG.Green(), kWHITE_BG.Blue() );
    combinedImage.InitAlpha();

    return new wxBitmap( combinedImage );
  }

  bool angleChangedToForceRedraw( double angle )
  {
    static int oldAngle = 0;
    return std::abs( oldAngle - static_cast< int >( angle ) ) > 0;
  }

  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
  wxButton* createButton( wxWindow* parent, const std::string& label, const std::string& tooltip, const int id,
    bool bindPressAndRelease = false )
  {
    auto* button = new wxButton( parent, id, label );
    button->SetToolTip( tooltip );
    return button;
  }

} // namespace

wxBEGIN_EVENT_TABLE( ControllerFrame, wxPanel ) EVT_PAINT( ControllerFrame::onPaint ) wxEND_EVENT_TABLE()

  ControllerFrame::ControllerFrame( wxWindow* parent, const Controller& controller, const int idOffset )
  : wxPanel( parent )
  , m_controller( controller )
  , m_timer( this, wxID_ANY )
{
  createButtons( idOffset );
  createLayout();

  Bind( wxEVT_TIMER, &ControllerFrame::onTimer, this, m_timer.GetId() );
}

ControllerFrame::~ControllerFrame()
{
  delete m_pos1Button;
  delete m_pos2Button;
  delete m_pos3Button;
  delete m_referenceButton;
  delete m_moveUpButton;
  delete m_moveDownButton;
  delete m_savePos1Button;
  delete m_savePos2Button;
  delete m_savePos3Button;
  delete m_tableHeightLabel;
  delete m_trainerBgImage;
  delete m_seatImage;
  delete m_imagePanel;
  m_timer.Stop();
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
  if( m_controller == nullptr )
  {
    return;
  }

  const int id = event.GetId();
  if( id == m_pos1Button->GetId() )
  {
    disableButtons();
    m_controller->moveToUserPosition( AXIS::ONE, USER_POSITION::POS_1 );
  }
  else if( id == m_pos2Button->GetId() )
  {
    disableButtons();
    m_controller->moveToUserPosition( AXIS::ONE, USER_POSITION::POS_2 );
  }
  else if( id == m_pos3Button->GetId() )
  {
    disableButtons();
    m_controller->moveToUserPosition( AXIS::ONE, USER_POSITION::POS_3 );
  }
  else if( id == m_referenceButton->GetId() )
  {
    disableButtons();
    m_controller->referenceRun( AXIS::ONE );
  }
  else if( id == m_savePos1Button->GetId() )
  {
    m_controller->saveUserPosition( AXIS::ONE, USER_POSITION::POS_1 );
  }
  else if( id == m_savePos2Button->GetId() )
  {
    m_controller->saveUserPosition( AXIS::ONE, USER_POSITION::POS_2 );
  }
  else if( id == m_savePos3Button->GetId() )
  {
    m_controller->saveUserPosition( AXIS::ONE, USER_POSITION::POS_3 );
  }
}

void ControllerFrame::onButtonPress( wxMouseEvent& event )
{
  if( m_controller == nullptr )
  {
    return;
  }

  const int id = event.GetId();
  if( id == m_moveUpButton->GetId() )
  {
    m_controller->setMoveUpDown( AXIS::ONE, USER_POSITION::MOVE_UP );
    disableButtons( true );
    m_timer.Start( kPOLLING_INTERVAL );
  }
  else if( id == m_moveDownButton->GetId() )
  {
    m_controller->setMoveUpDown( AXIS::ONE, USER_POSITION::MOVE_DOWN );
    disableButtons( true );
    m_timer.Start( kPOLLING_INTERVAL );
  }
  event.Skip();
}

void ControllerFrame::onButtonRelease( wxMouseEvent& event )
{
  if( m_controller == nullptr )
  {
    return;
  }

  const int id = event.GetId();
  if( id == m_moveUpButton->GetId() || id == m_moveDownButton->GetId() )
  {
    m_controller->setUpDownDisabled( AXIS::ONE );
    enableButtons();
  }
  event.Skip();
}

void ControllerFrame::enableButtons()
{
  m_referenceButton->Enable();

  m_moveUpButton->Enable();
  m_moveDownButton->Enable();

  m_pos1Button->Enable();
  m_pos2Button->Enable();
  m_pos3Button->Enable();

  m_savePos1Button->Enable();
  m_savePos2Button->Enable();
  m_savePos3Button->Enable();

  m_timer.Stop();
  updateTableHeight();
}

void ControllerFrame::enableButtons( wxCommandEvent& /*event*/ )
{
  enableButtons();
}

void ControllerFrame::disableButtons( bool onPress )
{
#ifndef WIN32
  if( !onPress )
  {
#endif
    m_moveUpButton->Disable();
    m_moveDownButton->Disable();
#ifndef WIN32
  }
#endif

  m_referenceButton->Disable();

  m_pos1Button->Disable();
  m_pos2Button->Disable();
  m_pos3Button->Disable();

  m_savePos1Button->Disable();
  m_savePos2Button->Disable();
  m_savePos3Button->Disable();
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

void ControllerFrame::rotateSeatImage( double angle )
{
  if( angleChangedToForceRedraw( angle ) )
  {
    m_rotatedSeatImage = rotateImage( m_seatImage, angle );

    wxRect invalidRect( kCENTER_SEAT.x - ( m_rotatedSeatImage.GetWidth() / 2 ),
      kCENTER_SEAT.y - ( m_rotatedSeatImage.GetHeight() / 2 ), m_rotatedSeatImage.GetWidth(),
      m_rotatedSeatImage.GetHeight() );
    RefreshRect( invalidRect );
    Update();
  }
}

void ControllerFrame::onPaint( wxPaintEvent& /*event*/ )
{
  wxAutoBufferedPaintDC dc( this );
  dc.SetBackground( *wxWHITE_BRUSH );
  dc.Clear();
  auto* combinedImage = combineBitmaps( m_trainerBgImage, &m_rotatedSeatImage );
  dc.DrawBitmap( *combinedImage, 0, 0, true );
  delete combinedImage;
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
    // the wxQueueEvent takes care of deleting the event created with new, so no memory leak here
    m_controller->setCallback( [ this ] { wxQueueEvent( this, new wxCommandEvent( EVT_ENABLE_BUTTONS ) ); } ); // NOLINT
  }
  Bind( EVT_ENABLE_BUTTONS, &ControllerFrame::enableButtons, this );
}

void ControllerFrame::createLayout()
{
  SetBackgroundStyle( wxBG_STYLE_PAINT );

  m_tableHeightLabel = new wxStaticText( this, wxID_ANY, "Table Height:" );
  updateTableHeight();

  m_imagePanel = new wxPanel( this, wxID_ANY );
  m_imagePanel->SetBackgroundStyle( wxBG_STYLE_PAINT );

  wxImage::AddHandler( new wxPNGHandler );
  m_trainerBgImage = createImage( m_imagePanel, "Trainer.png", false );
  m_seatImage = createImage( m_imagePanel, "Seat.png" );
  m_rotatedSeatImage = rotateImage( m_seatImage, 0 );

  auto* mainSizer = new wxBoxSizer( wxHORIZONTAL );
  auto* buttonSizer = new wxBoxSizer( wxVERTICAL );
  mainSizer->Add( m_imagePanel, 1, wxALIGN_TOP | wxALL, kBORDER );

  buttonSizer->Add( m_tableHeightLabel, 0, wxALL, kBORDER );
  buttonSizer->AddSpacer( kSMALL_SPACER );
  buttonSizer->Add( m_pos1Button, 0, wxALL, kBORDER );
  buttonSizer->Add( m_pos2Button, 0, wxALL, kBORDER );
  buttonSizer->Add( m_pos3Button, 0, wxALL, kBORDER );
  buttonSizer->AddStretchSpacer( 1 );
  buttonSizer->Add( m_moveUpButton, 0, wxALL, kBORDER );
  buttonSizer->Add( m_moveDownButton, 0, wxALL, kBORDER );
  buttonSizer->AddSpacer( kBUTTON_SPACER );
  buttonSizer->Add( m_savePos1Button, 0, wxALL, kBORDER );
  buttonSizer->Add( m_savePos2Button, 0, wxALL, kBORDER );
  buttonSizer->Add( m_savePos3Button, 0, wxALL, kBORDER );
  buttonSizer->AddSpacer( kBIG_SPACER );
  buttonSizer->Add( m_referenceButton, 0, wxALL, kBORDER );

  mainSizer->Add( buttonSizer, 0, wxEXPAND | wxALL, kBORDER );
  SetSizerAndFit( mainSizer );
}

void ControllerFrame::createButtons( const int idOffset )
{
  // NOLINTBEGIN(*-magic-numbers)
  m_moveUpButton = createButton( this, "Move Up", "Move up", wxID_ANY + idOffset + 3 );
  m_moveDownButton = createButton( this, "Move Down", "Move down", wxID_ANY + idOffset + 4 );

  m_referenceButton = createButton( this, "Reference", "Reference run", wxID_ANY + idOffset + 2 );

  m_pos1Button = createButton( this, "Pos 1", "Move to position 1", wxID_ANY + idOffset );
  m_pos2Button = createButton( this, "Pos 2", "Move to position 2", wxID_ANY + idOffset + 1 );
  m_pos3Button = createButton( this, "Pos 3", "Move to position 3", wxID_ANY + idOffset + 2 );

  m_savePos1Button = createButton( this, "Save Pos 1", "Save position 1", wxID_ANY + idOffset + 5 );
  m_savePos2Button = createButton( this, "Save Pos 2", "Save position 2", wxID_ANY + idOffset + 6 );
  m_savePos3Button = createButton( this, "Save Pos 3", "Save position 3", wxID_ANY + idOffset + 7 );
  // NOLINTEND(*-magic-numbers)

  bindButtonsPressAndRelease( m_moveUpButton );
  bindButtonsPressAndRelease( m_moveDownButton );

  bindButtons( m_referenceButton );

  bindButtons( m_pos1Button );
  bindButtons( m_pos2Button );
  bindButtons( m_pos3Button );

  bindButtons( m_savePos1Button );
  bindButtons( m_savePos2Button );
  bindButtons( m_savePos3Button );

  setBindingForReenablingButtons();
}
