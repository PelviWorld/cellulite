#include "ltccontroller.h"
#include "movetecmodbus.h"

#include <array>
#include <ltcregister.h>
#include <laingvalue.h>
#include <thread>

constexpr auto kMAX_USER_POSITIONS = 4;
constexpr auto kUP = 5;
constexpr auto kDOWN = 6;

std::unique_ptr< ILxcController > createController( const std::shared_ptr< MoveTecModBus >& modBus )
{
  return std::make_unique< LtcController >( modBus );
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

    bool moveInProgress() const
    {
      std::array< std::uint16_t, 1 > value{ 0 };
      modBus->readRegisters( static_cast< std::uint16_t >( LTC_REGISTER::MOVEMENT_IN_PROGRESS ), value );
      return value[ 0 ] == 1;
    }

    bool isMovementInProgress() const
    {
      constexpr auto kREGISTERS = 40;
      std::array< std::uint16_t, kREGISTERS > value{ 0 };
      modBus->readRegisters( static_cast< uint16_t >( LTC_REGISTER::GUI_SUPPORT_REGISTERS_START ), value );
      std::array< std::uint16_t, 1 > movement{ 0 };
      modBus->readRegisters( static_cast< std::uint16_t >( LTC_REGISTER::MOVEMENT_IN_PROGRESS ), movement );
      return movement[ 0 ] == static_cast< std::uint16_t >( SYSTEM_STATE::MOVE );
    }

    void moveToPosition( const uint16_t pos ) const
    {
      constexpr auto kPOLLING_INTERVAL = 100;
      constexpr auto kMOVEMENT_TIMEOUT = 1500;
      modBus->writeRegisters( static_cast< std::uint16_t >( LTC_REGISTER::KEYPRESS_CONTROL ), std::array{ pos } );
      std::this_thread::sleep_for( std::chrono::milliseconds( kPOLLING_INTERVAL ) );
      while(isMovementInProgress())
      {
        std::this_thread::sleep_for( std::chrono::milliseconds( kPOLLING_INTERVAL ) );
      }
      std::this_thread::sleep_for( std::chrono::milliseconds( kMOVEMENT_TIMEOUT ) );
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

void LtcController::moveToPosition( const std::uint16_t pos ) const
{
  if(pos <= kMAX_USER_POSITIONS)
  {
    m_pImpl->moveToPosition( pos );
  }
}
