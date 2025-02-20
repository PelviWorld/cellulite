#include "ltccontroller.h"
#include "movetecmodbus.h"

#include <array>
#include <fstream>
#include <laingvalue.h>
#include <ltcregister.h>
#include <thread>

namespace
{
  constexpr auto kDOWN = 6;
  constexpr auto kPOLLING_INTERVAL = 100;
  constexpr auto kMOVEMENT_TIMEOUT = 1500;
}

class LtcController::Impl
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

    bool isMovementInProgress() const
    {
      constexpr auto kREGISTERS = 40;
      std::array< std::uint16_t, kREGISTERS > value{ 0 };
      modBus->readRegisters( static_cast< uint16_t >( LTC_REGISTER::GUI_SUPPORT_REGISTERS_START ), value );
      std::array< std::uint16_t, 1 > movement{ 0 };
      modBus->readRegisters( static_cast< std::uint16_t >( LTC_REGISTER::MOVEMENT_IN_PROGRESS ), movement );
      return movement[ 0 ] == static_cast< std::uint16_t >( SYSTEM_STATE::MOVE );
    }

    void moveToPosition( const USER_POSITION pos ) const
    {
      modBus->writeRegisters(
        static_cast< std::uint16_t >( LTC_REGISTER::KEYPRESS_CONTROL ), std::array{ static_cast< uint16_t >( pos ) } );
      std::this_thread::sleep_for( std::chrono::milliseconds( kPOLLING_INTERVAL ) );
      while( isMovementInProgress() )
      {
        std::this_thread::sleep_for( std::chrono::milliseconds( kPOLLING_INTERVAL ) );
      }
      std::this_thread::sleep_for( std::chrono::milliseconds( kMOVEMENT_TIMEOUT ) );
    }

    void referenceRun() const
    {
      std::array< std::uint16_t, 1 > value{ kDOWN };
      modBus->writeRegister(
        static_cast< uint16_t >( LTC_REGISTER::SYSTEM_MODE ), static_cast< std::uint16_t >( SYSTEM_MODE::CLEAR ) );
      modBus->writeRegister(
        static_cast< uint16_t >( LTC_REGISTER::SYSTEM_MODE ), static_cast< std::uint16_t >( SYSTEM_MODE::REFERENCE ) );
      modBus->writeRegisters( static_cast< std::uint16_t >( LTC_REGISTER::KEYPRESS_CONTROL ), value );
      std::this_thread::sleep_for( std::chrono::milliseconds( kPOLLING_INTERVAL ) );
      while( isMovementInProgress() )
      {
        std::this_thread::sleep_for( std::chrono::milliseconds( kPOLLING_INTERVAL ) );
      }
      std::this_thread::sleep_for( std::chrono::milliseconds( kMOVEMENT_TIMEOUT ) );
      modBus->readRegisters( static_cast< std::uint16_t >( LTC_REGISTER::SYSTEM_MODE ), value );
    }

    void setUpDownDisabled() const
    {
      modBus->writeRegister( static_cast< uint16_t >( LTC_REGISTER::KEYPRESS_CONTROL ),
        static_cast< std::uint16_t >( USER_POSITION::STOP ) );
    }

    void moveUpDown( USER_POSITION moveDirection ) const
    {
      modBus->writeRegister(
        static_cast< uint16_t >( LTC_REGISTER::KEYPRESS_CONTROL ), static_cast< std::uint16_t >( moveDirection ) );
    }

    uint16_t getUserPositionRegister( const USER_POSITION pos ) const
    {
      switch( pos )
      {
        case USER_POSITION::POS_1:
          return static_cast< uint16_t >( LTC_REGISTER::USER_POSITION_1 );
        case USER_POSITION::POS_2:
          return static_cast< uint16_t >( LTC_REGISTER::USER_POSITION_2 );
        case USER_POSITION::POS_3:
          return static_cast< uint16_t >( LTC_REGISTER::USER_POSITION_3 );
        case USER_POSITION::POS_4:
          return static_cast< uint16_t >( LTC_REGISTER::USER_POSITION_4 );
        default:
          break;
      }
      return 0;
    }

    void saveUserPosition( const USER_POSITION pos ) const
    {
      const uint16_t address = getUserPositionRegister( pos );
      if( address == 0 )
      {
        return;
      }
      const auto height = getTableHeight();
      modBus->writeRegister( address, height );
    }

    uint16_t getTableHeight() const
    {
      std::array< uint16_t, 1 > value{ 0 };
      modBus->readRegisters( static_cast< uint16_t >( LTC_REGISTER::TABLE_HEIGHT ), value );
      return value[ 0 ];
    }

    const std::shared_ptr< MoveTecModBus > modBus;
};

LtcController::LtcController( const std::shared_ptr< MoveTecModBus >& modbus )
  : m_pImpl{ std::make_unique< Impl >( modbus ) }
{
}

LtcController::~LtcController()
{
}

void LtcController::moveToUserPosition( AXIS /*axis*/, const USER_POSITION pos ) const
{
  m_pImpl->moveToPosition( pos );
}

void LtcController::referenceRun( AXIS /*axis*/ ) const
{
  m_pImpl->referenceRun();
}

uint16_t LtcController::getTableHeight( AXIS /*axis*/ ) const
{
  return m_pImpl->getTableHeight();
}
void LtcController::setUpDownDisabled( AXIS /*axis*/ ) const
{
  m_pImpl->setUpDownDisabled();
}
void LtcController::setMoveUpDown( AXIS /*axis*/, const USER_POSITION moveDirection )
{
  m_pImpl->moveUpDown( moveDirection );
}
void LtcController::saveUserPosition( AXIS /*axis*/, const USER_POSITION pos ) const
{
  m_pImpl->saveUserPosition( pos );
}
