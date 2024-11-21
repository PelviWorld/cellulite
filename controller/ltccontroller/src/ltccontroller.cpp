#include "ltccontroller.h"
#include "movetecmodbus.h"

#include <array>
#include <ltcregister.h>
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
      constexpr auto kPOLL_REGISTER = 10000;
      constexpr auto kMOTOR_ONE_PWR = 14;
      std::array< std::uint16_t, kREGISTERS > value{ 0 };
      modBus->readRegisters( kPOLL_REGISTER, value );
      return value[ kMOTOR_ONE_PWR ] != 0;
    }

    void moveToPosition( const uint16_t pos ) const
    {
      modBus->writeRegisters( static_cast< std::uint16_t >( LTC_REGISTER::KEYPRESS_CONTROL ), std::array{ pos } );
      std::this_thread::sleep_for( std::chrono::milliseconds( 150 ) );
      while(isMovementInProgress())
      {
      }
      std::this_thread::sleep_for( std::chrono::milliseconds( 1500 ) );
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
