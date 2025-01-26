#pragma once

// laingvalue is needed
#include "laingvalue.h"

#include "celluliteframe.h"
#include "gyrocom.h"

#include <atomic>
#include <thread>
#include <wx/wx.h>

class INIReader;

class CelluliteApp final : public wxApp
{
  public:
    bool OnInit() override;
    int OnExit() override;

  private:
    void readDataFromPico();
    void readLoop();

    std::unique_ptr< CelluliteFrame > m_frame{ nullptr };
    std::unique_ptr< GyroCom > m_gyroCom;
    wxStaticText* m_actualPitchLabel{ nullptr };
    std::thread readThread;
    std::atomic< bool > running{ true };
};
