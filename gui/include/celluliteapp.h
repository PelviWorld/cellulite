#pragma once

#include <laingcontroller.h>
#include <wx/wx.h>

enum class ControllerAxis
{
  HEIGHT,
  WIDTH,
};

class CelluliteApp final : public wxApp
{
  public:
    bool OnInit() override;
    LaingController& getController( ControllerAxis axis );

  private:
    std::unordered_map< ControllerAxis, LaingController > m_controller;
};
