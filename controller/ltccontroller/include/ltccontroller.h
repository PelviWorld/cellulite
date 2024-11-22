#pragma once
#include "ilxccontroller.h"

#include <memory>

class MoveTecModBus;

class LtcController final : public ILxcController
{
  public:
    explicit LtcController( const std::shared_ptr< MoveTecModBus >& modbus );

    ~LtcController() override;

    LtcController( const LtcController& ) = delete;

    LtcController& operator=( const LtcController& ) = delete;

    LtcController( LtcController&& ) = delete;

    LtcController& operator=( LtcController&& ) = delete;

    void moveToUserPosition( USER_POSITION pos ) const override;

    void referenceRun() const override;

  private:
    class Impl;
    std::unique_ptr< Impl > m_pImpl;
};

extern "C" {
std::unique_ptr< ILxcController > createController( const std::shared_ptr< MoveTecModBus >& modBus );
}
