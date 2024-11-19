#pragma once
#include <memory>

class MoveTecModBus;

class LaingController
{
  public:
    explicit LaingController( const std::shared_ptr< MoveTecModBus >& modbus );

    ~LaingController();

    LaingController( const LaingController& ) = delete;

    LaingController& operator=( const LaingController& ) = delete;

    LaingController( LaingController&& ) = delete;

    LaingController& operator=( LaingController&& ) = delete;

  private:
    class Impl;
    std::unique_ptr< Impl > m_pImpl;
};
