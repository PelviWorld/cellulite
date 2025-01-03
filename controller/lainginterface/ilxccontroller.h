#pragma once

#include <laingvalue.h>
#include <mutex>

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

    virtual void moveToUserPosition( AXIS axis, USER_POSITION pos ) const = 0;
    virtual void referenceRun( AXIS axis ) const = 0;
    virtual void setMoveUpDown( AXIS axis, USER_POSITION user_position ) = 0;
    virtual void setUpDownDisabled( AXIS axis ) const = 0;
    virtual std::uint16_t getTableHeight( AXIS axis ) const = 0;

    std::mutex& getMutex() const
    {
      return mtx;
    }
    std::condition_variable& getConditionVariable() const
    {
      return cv;
    }
    bool& getIsRunning() const
    {
      return isRunning;
    }

  protected:
    mutable std::mutex mtx;
    mutable std::condition_variable cv;
    mutable bool isRunning{ false };
};
