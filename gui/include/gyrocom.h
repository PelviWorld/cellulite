// gyrocom.h
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <string>
#include <windows.h>

class GyroCom
{
  public:
    explicit GyroCom( const std::string& portName );
    ~GyroCom();
    bool verifyConnection();
    std::string readData();

  private:
    HANDLE hSerial;
    void writeData( const std::string& data );
    void configurePort();
};