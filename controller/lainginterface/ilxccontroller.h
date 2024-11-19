#pragma once

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

    virtual void moveToPosition( std::uint16_t pos ) const = 0;
};
