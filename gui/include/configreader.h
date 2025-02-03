#pragma once

#include "INIReader.h"
#include "controller.h"
#include <unordered_map>

INIReader getReader( const std::string& filename );
std::unordered_map< ControllerAxis, int > readSerialConfig( const INIReader& reader );
std::vector< std::string > createController( ControllerMap& controllerMap );