#pragma once
#include "controller.h"
#include "startframe.h"
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

  private:
    void onHello( wxCommandEvent& event );
    void onExit( wxCommandEvent& event );
    void onAbout( wxCommandEvent& event );

    StartFrame* m_heightFrame{ nullptr };
    StartFrame* m_widthFrame{ nullptr };
};