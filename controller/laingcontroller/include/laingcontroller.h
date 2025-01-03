#pragma once
#include <memory>
#include <string>

#include "laingvalue.h"

#include <functional>
#include <ilxccontroller.h>

class MoveTecModBus;

class LaingController final
{
  public:
    LaingController();
    explicit LaingController( const std::string& device );

    LaingController( const LaingController& ) = delete;

    LaingController& operator=( const LaingController& ) = delete;

    LaingController( LaingController&& ) = delete;

    LaingController& operator=( LaingController&& ) = delete;

    ~LaingController();

    void setDevice( const std::string& device );

    void setCallback( std::function< void() > callback );

    std::uint16_t getTableHeight( AXIS axis ) const;

    void moveToUserPosition( AXIS axis, USER_POSITION pos ) const;

    void referenceRun( AXIS axis ) const;

    int getSerialNumber() const;

  private:
    std::string m_device;
    class Impl;
    std::unique_ptr< Impl > m_pImpl;
};
