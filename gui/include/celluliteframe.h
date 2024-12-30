#pragma once
#include <wx/wx.h>

enum
{
  ID_HELLO = 1,
  ID_TRAINING = 2,
};

class CelluliteFrame final : public wxFrame
{
  public:
    CelluliteFrame();

  private:
    void onHello( wxCommandEvent& event );
    void onExit( wxCommandEvent& event );
    void onAbout( wxCommandEvent& event );
    void onTraining( wxCommandEvent& event );
};
