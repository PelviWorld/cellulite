#include "lmccontroller.h"
#include "laingvalue.h"
#include "lmcregister.h"
#include "movetecmodbus.h"

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
    }

    void referenceRun( AXIS axis ) const
    {
    }

    uint16_t getTableHeight( AXIS axis ) const
    {
      return 0;
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
