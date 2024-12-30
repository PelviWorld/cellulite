#pragma once

#include <laingcontroller.h>
#include <wx/wx.h>

class INIReader;

enum class ControllerAxis
{
  HEIGHT,
  WIDTH,
};

class CelluliteApp final : public wxApp
{
  public:
    bool OnInit() override;
    std::shared_ptr< LaingController > getController( ControllerAxis axis );

  private:
    void createControllerMap( const INIReader& reader, const std::unordered_map< ControllerAxis, int >& serialConfig );
    void createController();
    void moveControllerToMap(
      std::shared_ptr< LaingController > controller, const std::unordered_map< ControllerAxis, int >& serialConfig );

    std::unordered_map< ControllerAxis, std::shared_ptr< LaingController > > m_controller;
};
