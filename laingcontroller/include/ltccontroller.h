#pragma once
#include <memory>

class MoveTecModBus;

class LtcController
{
  public:
    explicit LtcController( const std::shared_ptr< MoveTecModBus >& modbus );

    ~LtcController();

    LtcController( const LtcController& ) = delete;

    LtcController& operator=( const LtcController& ) = delete;

    LtcController( LtcController&& ) = delete;

    LtcController& operator=( LtcController&& ) = delete;

    void moveToPosition( uint16_t pos ) const;

  private:
    class Impl;
    std::unique_ptr< Impl > m_pImpl;
};
