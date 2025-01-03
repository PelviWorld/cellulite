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

    void moveToUserPosition( AXIS axis, USER_POSITION pos ) const override;

    void referenceRun( AXIS axis ) const override;

    uint16_t getTableHeight( AXIS axis ) const override;

    void setUpDownDisabled( AXIS axis ) const override;

    void setMoveUpDown( AXIS axis, USER_POSITION moveDirection ) override;

    void saveUserPosition( AXIS axis, USER_POSITION pos ) const override;

  private:
    class Impl;
    std::unique_ptr< Impl > m_pImpl;
};
