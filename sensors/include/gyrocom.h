#pragma once

#define WIN32_LEAN_AND_MEAN
#include <memory>

class GyroCom
{
  public:
    explicit GyroCom( double pos1, double pos2, double pos3 );
    ~GyroCom();

    GyroCom( const GyroCom& ) = delete;
    GyroCom( GyroCom&& ) = delete;
    GyroCom& operator=( const GyroCom& ) = delete;
    GyroCom& operator=( GyroCom&& ) = delete;

    double readDouble();
    void setPosition( int pos );

  private:
    class Impl;
    std::unique_ptr< Impl > m_pImpl;
};
