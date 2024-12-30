#pragma once

#include "laingcontroller.h"
#include <unordered_map>

enum class ControllerAxis
{
  HEIGHT,
  WIDTH,
};

using ControllerMap = std::unordered_map< ControllerAxis, std::shared_ptr< LaingController > >;