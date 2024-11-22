#pragma once
#include <laingvalue.h>
#include <string>

class MoveTecModBus;

class ILaingController
{
  public:
    ILaingController() = default;

    explicit ILaingController( const std::string& device )
    {
    }

    virtual ~ILaingController() = default;

    ILaingController( const ILaingController& ) = delete;

    ILaingController& operator=( const ILaingController& ) = delete;

    ILaingController( ILaingController&& ) = delete;

    ILaingController& operator=( ILaingController&& ) = delete;

    virtual std::uint16_t getTableHeight( AXIS axis ) const = 0;

    virtual void moveToUserPosition( AXIS axis, USER_POSITION pos ) const = 0;

    virtual void referenceRun( AXIS axis ) const = 0;
};
