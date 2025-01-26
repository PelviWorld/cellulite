#include "lmccontroller.h"
#include "laingvalue.h"
#include "lmcregister.h"
#include "movetecmodbus.h"

#include <array>
#include <iostream>

namespace
{
  constexpr auto kMAX_USER_POSITIONS = 4;
  constexpr auto kUP = 5;
  constexpr auto kDOWN = 6;
}

class LmcController::Impl
{
  public:
    explicit Impl( const std::shared_ptr< MoveTecModBus >& modBus_ )
      : modBus{ modBus_ }
    {
    }

    ~Impl() = default;

    Impl( const Impl& ) = delete;

    Impl& operator=( const Impl& ) = delete;

    Impl( Impl&& ) = delete;

    Impl& operator=( Impl&& ) = delete;

    static uint16_t getChannelRegisterOffset( LMC_REGISTER address, AXIS axis, const uint16_t baseOffset = 80 )
    {
      return static_cast< uint16_t >( address ) + ( baseOffset * ( static_cast< uint16_t >( axis ) - 1 ) );
    }

    bool isMovementInProgress() const
    {
      return true;
    }

    void moveToPosition( const AXIS axis, const USER_POSITION pos ) const
    {
      modBus->writeRegister( 3016, 0 );
      modBus->writeRegister( 3506, 0 );
    }

    void referenceRun( AXIS axis ) const
    {
      modBus->writeRegister( 3016, 0 );
      modBus->writeRegister( 3500, 6 );
      modBus->writeRegister( 3504, 1 );
    }

    uint16_t getTableHeight( AXIS axis ) const
    {
      modBus->writeRegister( 3016, 0 );
      std::array< uint16_t, 1 > value{ 0 };
      modBus->readRegisters( 11503, value );
      return value[ 0 ];
    }

    void setUpDownDisabled() const
    {
    }

    void moveUpDown( USER_POSITION moveDirection ) const
    {
    }

    void saveUserPosition( const USER_POSITION pos ) const
    {
      const uint16_t position = getTableHeight( AXIS::ONE );
      std::cout << "Position: " << position << std::endl;
    }

    const std::shared_ptr< MoveTecModBus > modBus;
};

LmcController::LmcController( const std::shared_ptr< MoveTecModBus >& modbus )
  : m_pImpl{ std::make_unique< Impl >( modbus ) }
{
}

LmcController::~LmcController()
{
}

void LmcController::moveToUserPosition( const AXIS axis, const USER_POSITION pos ) const
{
  m_pImpl->moveToPosition( axis, pos );
}

void LmcController::referenceRun( const AXIS axis ) const
{
  m_pImpl->referenceRun( axis );
}

uint16_t LmcController::getTableHeight( const AXIS axis ) const
{
  return m_pImpl->getTableHeight( axis );
}
void LmcController::setUpDownDisabled( AXIS /*axis*/ ) const
{
  m_pImpl->setUpDownDisabled();
}
void LmcController::setMoveUpDown( AXIS /*axis*/, const USER_POSITION moveDirection )
{
  m_pImpl->moveUpDown( moveDirection );
}
void LmcController::saveUserPosition( AXIS /*axis*/, const USER_POSITION pos ) const
{
  m_pImpl->saveUserPosition( pos );
}