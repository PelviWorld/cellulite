#pragma once

// laingvalue is needed
#include "laingvalue.h"

#include <wx/wx.h>

class INIReader;

class CelluliteApp final : public wxApp
{
  public:
    bool OnInit() override;
};
