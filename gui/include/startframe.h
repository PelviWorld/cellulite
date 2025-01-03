#pragma once

#include "controller.h"

#include <wx/wx.h>

class StartFrame final : public wxPanel
{
  public:
    StartFrame( wxWindow* parent, const Controller& controller, int idOffset );

    void onHoverEnter( wxMouseEvent& event );
    void onHoverLeave( wxMouseEvent& event );
    void onPos1( wxCommandEvent& event );
    void onPos2( wxCommandEvent& event );
    void onReference( wxCommandEvent& event );
    void onTraining( wxCommandEvent& event );
    void enableButtons( wxCommandEvent& event );
    void disableButtons();

  private:
    wxButton* m_pos1Button{ nullptr };
    wxButton* m_pos2Button{ nullptr };
    wxButton* m_referenceButton{ nullptr };

    Controller m_controller;
};
