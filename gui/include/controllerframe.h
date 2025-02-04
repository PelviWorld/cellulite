#pragma once

#include "controller.h"

#include <wx/wx.h>

class ControllerFrame final : public wxPanel
{
  public:
    ControllerFrame( wxWindow* parent, const Controller& controller, int idOffset );

    void onHoverEnter( wxMouseEvent& event );
    void onHoverLeave( wxMouseEvent& event );
    void onClicked( wxCommandEvent& event );
    void onButtonPress( wxMouseEvent& event );
    void onButtonRelease( wxMouseEvent& event );
    void enableButtons( wxCommandEvent& event );
    void enableButtons();
    void disableButtons();
    void updateTableHeight();
    void rotateSeatImage( double angle );

  private:
    void createButtons( int idOffset );
    void setBindingForReenablingButtons();
    void bindButtons( wxButton* button );
    void bindButtonsPressAndRelease( wxButton* button );
    void bindHoverEvents( wxButton* button );
    void onTimer( wxTimerEvent& event );
    void onPaint( wxPaintEvent& event );

    wxButton* m_pos1Button{ nullptr };
    wxButton* m_pos2Button{ nullptr };
    wxButton* m_referenceButton{ nullptr };
    wxButton* m_moveUpButton{ nullptr };
    wxButton* m_moveDownButton{ nullptr };
    wxButton* m_savePos1Button{ nullptr };
    wxButton* m_savePos2Button{ nullptr };
    wxStaticText* m_tableHeightLabel{ nullptr };
    wxTimer m_timer;

    Controller m_controller;

    wxImage* m_trainerBgImage{ nullptr };
    wxImage* m_seatImage{ nullptr };
    wxImage m_rotatedSeatImage{};
    wxPanel* m_imagePanel{ nullptr };

    wxDECLARE_EVENT_TABLE();
};
