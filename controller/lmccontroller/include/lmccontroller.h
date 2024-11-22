#pragma once
#include "ilxccontroller.h"

#include <memory>

class MoveTecModBus;

class LmcController final : public ILxcController
{
  public:
    explicit LmcController( const std::shared_ptr< MoveTecModBus >& modbus );

    ~LmcController() override;

    LmcController( const LmcController& ) = delete;

    LmcController& operator=( const LmcController& ) = delete;

    LmcController( LmcController&& ) = delete;

    LmcController& operator=( LmcController&& ) = delete;

    void moveToUserPosition( USER_POSITION pos ) const override;

  private:
    class Impl;
    std::unique_ptr< Impl > m_pImpl;
};

extern "C" {
std::unique_ptr< ILxcController > createController( const std::shared_ptr< MoveTecModBus >& modBus );
}
