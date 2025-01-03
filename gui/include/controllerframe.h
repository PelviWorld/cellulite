#pragma once

#include "controller.h"

#include <wx/wx.h>

class ControllerFrame final : public wxPanel
{
  public:
    ControllerFrame( wxWindow* parent, const Controller& controller, int idOffset );

    void onHoverEnter( wxMouseEvent& event );
    void onHoverLeave( wxMouseEvent& event );
    void onPos1( wxCommandEvent& event );
    void onPos2( wxCommandEvent& event );
    void onReference( wxCommandEvent& event );
    void enableButtons( wxCommandEvent& event );
    void disableButtons();
    void updateTableHeight();

  private:
    void createButtons( int idOffset );
    void setBindingForReenablingButtons();
    void bindButtons( wxButton* button );

    wxButton* m_pos1Button{ nullptr };
    wxButton* m_pos2Button{ nullptr };
    wxButton* m_referenceButton{ nullptr };
    wxStaticText* m_tableHeightLabel{ nullptr };

    Controller m_controller;
};
