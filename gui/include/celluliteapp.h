#pragma once

#include <wx/wx.h>

enum
{
  ID_Hello = 1
};

class CelluliteApp : public wxApp
{
  public:
    bool OnInit() override;
};

class CelluliteFrame : public wxFrame
{
  public:
    CelluliteFrame();

  private:
    void onHello( wxCommandEvent& event );
    void onExit( wxCommandEvent& event );
    void onAbout( wxCommandEvent& event );
};
