#pragma once
#include "controller.h"
#include "controllerframe.h"
#include <wx/wx.h>

enum
{
  ID_HELLO = 1,
};

class CelluliteFrame final : public wxFrame
{
  public:
    explicit CelluliteFrame( const ControllerMap& controllerMap );

    void createMenuBar();
    void crateStatusBar();
    void updateSaveButtons( int value );

  private:
    void onHello( wxCommandEvent& event );
    void onExit( wxCommandEvent& event );
    void onAbout( wxCommandEvent& event );

    ControllerFrame* m_heightFrame{ nullptr };
    ControllerFrame* m_widthFrame{ nullptr };
};