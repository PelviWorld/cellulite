#pragma once

#include "INIReader.h"
#include "controller.h"
#include "gyrocom.h"
#include <unordered_map>

INIReader getReader( const std::string& filename );
std::unordered_map< ControllerAxis, int > readSerialConfig( const INIReader& reader );
std::vector< std::string > createController( ControllerMap& controllerMap );
std::vector< std::string > createControllerMapAndReturnRemainingPorts( ControllerMap& controllerMap );
std::shared_ptr< GyroCom > createGyro();
