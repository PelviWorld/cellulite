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

    void moveToUserPosition( AXIS axis, USER_POSITION pos ) const override;

    void referenceRun( AXIS axis ) const override;

    uint16_t getTableHeight( AXIS axis ) const override;
    void setMoveUpDown( AXIS axis, USER_POSITION user_position ) override {};
    void setUpDownDisabled( AXIS axis ) const override {};

  private:
    class Impl;
    std::unique_ptr< Impl > m_pImpl;
};
