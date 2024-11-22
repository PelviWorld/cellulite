#pragma once

#include <laingvalue.h>
#include <memory>
class MoveTecModBus;

class ILxcController
{
  public:
    ILxcController() = default;

    explicit ILxcController( const std::shared_ptr< MoveTecModBus >& modBus )
    {
    }

    virtual ~ILxcController() = default;

    ILxcController( const ILxcController& ) = delete;

    ILxcController& operator=( const ILxcController& ) = delete;

    ILxcController( ILxcController&& ) = delete;

    ILxcController& operator=( ILxcController&& ) = delete;

    virtual void moveToUserPosition( USER_POSITION pos ) const = 0;
};
