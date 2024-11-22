#include "lmccontroller.h"
#include "movetecmodbus.h"
#include "lmcregister.h"

#include <array>

constexpr auto kMAX_USER_POSITIONS = 4;
constexpr auto kUP = 5;
constexpr auto kDOWN = 6;

std::unique_ptr< ILxcController > createController( const std::shared_ptr< MoveTecModBus >& modBus )
{
  return std::make_unique< LmcController >( modBus );
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

    bool moveInProgress() const
    {
      std::array< std::uint16_t, 1 > value{ 0 };
      modBus->readRegisters( static_cast< std::uint16_t >( LMC_REGISTER::MOVEMENT_IN_PROGRESS ), value );
      return value[ 0 ] == 1;
    }

    bool isMovementInProgress() const
    {
      std::array< std::uint16_t, 1 > value{ 0 };
      modBus->readRegisters( static_cast< std::uint16_t >( LMC_REGISTER::MOVEMENT_IN_PROGRESS ), value );
      return value[ 0 ] == 1;
    }

    void moveToPosition( const USER_POSITION pos ) const
    {
      modBus->writeRegisters( static_cast< std::uint16_t >( LMC_REGISTER::KEYPRESS_CONTROL ),
        std::array{ static_cast< uint16_t >( pos ) } );
      while(isMovementInProgress())
      {
      }
      sleep( 2 );
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

void LmcController::moveToUserPosition( USER_POSITION pos ) const
{
  m_pImpl->moveToPosition( pos );
}
