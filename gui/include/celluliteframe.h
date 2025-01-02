#pragma once
#include "controller.h"
#include <wx/wx.h>

enum
{
  ID_HELLO = 1,
  ID_TRAINING,
  ID_POS_1,
  ID_POS_2,
  ID_REFERENCE
};

class CelluliteFrame final : public wxFrame
{
  public:
    explicit CelluliteFrame( const ControllerMap& controllerMap );

    void createMenuBar();
    void crateStatusBar();
    void createStartFrame();

  private:
    void onHoverEnter( wxMouseEvent& event );
    void onHoverLeave( wxMouseEvent& event );

    void onHello( wxCommandEvent& event );
    void onExit( wxCommandEvent& event );
    void onAbout( wxCommandEvent& event );
    void onTraining( wxCommandEvent& event );
    void onPos1( wxCommandEvent& event );
    void onPos2( wxCommandEvent& event );
    void onReference( wxCommandEvent& event );
    void enableButtons( wxCommandEvent& event );
    void disableButtons();

    wxButton* m_pos1Button{ nullptr };
    wxButton* m_pos2Button{ nullptr };
    wxButton* m_referenceButton{ nullptr };

    ControllerMap m_controllerMap;
};
