#pragma once

#include <wx/wx.h>

enum
{
  ID_HELLO = 1
};

class CelluliteApp final : public wxApp
{
  public:
    bool OnInit() override;
};

class CelluliteFrame final : public wxFrame
{
  public:
    CelluliteFrame();

  private:
    void onHello( wxCommandEvent& event );
    void onExit( wxCommandEvent& event );
    void onAbout( wxCommandEvent& event );
};
