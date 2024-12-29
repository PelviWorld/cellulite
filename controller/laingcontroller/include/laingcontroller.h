#pragma once
#include <memory>
#include <string>

#include "laingvalue.h"

class MoveTecModBus;

class LaingController final
{
  public:
    explicit LaingController( const std::string& device );

    LaingController( const LaingController& ) = delete;

    LaingController& operator=( const LaingController& ) = delete;

    LaingController( LaingController&& ) = delete;

    LaingController& operator=( LaingController&& ) = delete;

    ~LaingController();

    std::uint16_t getTableHeight( AXIS axis ) const;

    void moveToUserPosition( AXIS axis, USER_POSITION pos ) const;

    void referenceRun( AXIS axis ) const;

  private:
    class Impl;
    std::unique_ptr< Impl > m_pImpl;
};
