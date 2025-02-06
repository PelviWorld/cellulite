#include "celluliteframe.h"

#include "cg_version.h"
#include <wx/dcbuffer.h>

namespace
{
  constexpr auto kWINDOW_WIDTH = 480;
  constexpr auto kWINDOW_HEIGHT = 800;
  const std::string kAPP_NAME = "Cellulite APP";
  constexpr auto kHEIGHT_ID_OFFSET = 0;
  constexpr auto kFRAME_BORDER = 5;
  const auto kWHITE_BG = wxColour( 255, 255, 255 );
}

wxBEGIN_EVENT_TABLE( CelluliteFrame, wxFrame ) EVT_PAINT( CelluliteFrame::onPaint ) wxEND_EVENT_TABLE()

  CelluliteFrame::CelluliteFrame( const ControllerMap& controllerMap )
  : wxFrame( nullptr, wxID_ANY, kAPP_NAME, wxDefaultPosition, wxSize( kWINDOW_WIDTH, kWINDOW_HEIGHT ),
      wxDEFAULT_FRAME_STYLE & ~( wxRESIZE_BORDER | wxMAXIMIZE_BOX ) )
{
  wxTopLevelWindowBase::SetMinSize( wxSize( kWINDOW_WIDTH, kWINDOW_HEIGHT ) );
  wxTopLevelWindowBase::SetMaxSize( wxSize( kWINDOW_WIDTH, kWINDOW_HEIGHT ) );
  createMenuBar();
  crateStatusBar();
  SetBackgroundStyle( wxBG_STYLE_PAINT );
  SetBackgroundColour( kWHITE_BG );

  const auto controllerHeight = controllerMap.find( ControllerAxis::HEIGHT );

  if( controllerHeight != controllerMap.end() )
  {
    m_heightFrame = new ControllerFrame( this, controllerHeight->second, kHEIGHT_ID_OFFSET );
  }
  else
  {
    m_heightFrame = new ControllerFrame( this, std::make_shared< LaingController >(), kHEIGHT_ID_OFFSET );
  }

  auto* sizer = new wxBoxSizer( wxVERTICAL );
  auto* headerSizer = new wxBoxSizer( wxHORIZONTAL );
  auto* frameSizer = new wxBoxSizer( wxHORIZONTAL );

  frameSizer->Add( m_heightFrame, 1, wxEXPAND | wxALL, kFRAME_BORDER );

  sizer->Add( headerSizer, 0, wxEXPAND );
  sizer->Add( frameSizer, 1, wxEXPAND );

  SetSizer( sizer );
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

void CelluliteFrame::setPitch( double pitch )
{
  m_heightFrame->rotateSeatImage( pitch );
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

void CelluliteFrame::onHello( wxCommandEvent& event )
{
  wxLogMessage( ( "Hello from your " + kAPP_NAME + "!" ).c_str() );
}

void CelluliteFrame::onPaint( wxPaintEvent& event )
{
  wxAutoBufferedPaintDC dc( this );
  dc.Clear();

  wxRegion updateRegion = GetUpdateRegion();
  wxRect controllerFrameRect = m_heightFrame->GetRect();
  updateRegion.Subtract( controllerFrameRect );

  wxRect updateRect = updateRegion.GetBox();

  dc.SetBrush( *wxWHITE_BRUSH );
  dc.SetPen( *wxTRANSPARENT_PEN );

  dc.DrawRectangle( updateRect );
}