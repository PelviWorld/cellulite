#pragma once

#include <laingcontroller.h>
#include <wx/wx.h>

class INIReader;

enum class ControllerAxis
{
  HEIGHT,
  WIDTH,
};

using ControllerMap = std::unordered_map< ControllerAxis, std::shared_ptr< LaingController > >;

class CelluliteApp final : public wxApp
{
  public:
    bool OnInit() override;
    std::shared_ptr< LaingController > getController( ControllerAxis axis );

  private:
    ControllerMap m_controller;
};
