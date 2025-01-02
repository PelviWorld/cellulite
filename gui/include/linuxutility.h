#pragma once

#include "INIReader.h"
#include <string>
#include <vector>

std::vector< std::string > getAvailableComPorts( const INIReader& reader );
std::string getString( const INIReader& reader, const std::string& section, const std::string& key );
