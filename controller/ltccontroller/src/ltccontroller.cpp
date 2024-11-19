#include "ltccontroller.h"
#include "movetecmodbus.h"

#include <array>
#include <ltcregister.h>

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
      std::array< std::uint16_t, 1 > value{ 0 };
      modBus->readRegisters( static_cast< std::uint16_t >( LTC_REGISTER::MOVEMENT_IN_PROGRESS ), value );
      return value[ 0 ] == 1;
    }

    void moveToPosition( const uint16_t pos ) const
    {
      modBus->writeRegisters( static_cast< std::uint16_t >( LTC_REGISTER::KEYPRESS_CONTROL ), std::array{ pos } );
      while(isMovementInProgress())
      {
        modBus->writeRegisters( static_cast< std::uint16_t >( LTC_REGISTER::KEYPRESS_CONTROL ), std::array{ pos } );
      }
      sleep( 2 );
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
