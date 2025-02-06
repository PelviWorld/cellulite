#pragma once

#define WIN32_LEAN_AND_MEAN
#include <memory>
#include <string>

class GyroCom
{
  public:
    explicit GyroCom( const std::string& portName );
    ~GyroCom();

    bool verifyConnection();
    double readDouble();

  private:
    class Impl;
    std::unique_ptr< Impl > m_pImpl;
};