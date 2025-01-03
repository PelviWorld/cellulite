#pragma once

#include "laingcontroller.h"
#include <unordered_map>

enum class ControllerAxis
{
  HEIGHT,
  WIDTH,
};

using Controller = std::shared_ptr< LaingController >;
using ControllerMap = std::unordered_map< ControllerAxis, Controller >;
