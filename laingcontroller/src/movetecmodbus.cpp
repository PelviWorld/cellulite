#include "movetecmodbus.h"

#include <array>
#include <modbus/modbus.h>
#include <iostream>

constexpr auto kBAUDRATE = 57600;
constexpr auto kDATA_BITS = 8;
constexpr auto kSTOP_BITS = 2;

class MoveTecModBus::Impl
{
  public:
    explicit Impl( const std::string& port )
      : ctx{ modbus_new_rtu( port.c_str(), kBAUDRATE, 'N', kDATA_BITS, kSTOP_BITS ) }
    {
      modbus_set_slave( ctx, 1 );
      if(modbus_connect( ctx ) == -1)
      {
        std::cerr << "Connection failed: " << modbus_strerror( errno ) << std::endl;
        modbus_free( ctx );
        ctx = nullptr;
      }
    }

    ~Impl()
    {
      if(ctx != nullptr)
      {
        modbus_close( ctx );
        modbus_free( ctx );
        ctx = nullptr;
      }
    }

    Impl( const Impl& ) = delete;

    Impl& operator=( const Impl& ) = delete;

    Impl( Impl&& ) = delete;

    Impl& operator=( Impl&& ) = delete;


    modbus_t* ctx;
};

MoveTecModBus::MoveTecModBus( const std::string& port )
  : m_pImpl{ std::make_unique< Impl >( port ) }
{
}

MoveTecModBus::~MoveTecModBus() = default;

bool MoveTecModBus::writeRegister( const uint16_t address, const uint16_t value )
{
  return writeRegisters( address, std::array< uint16_t, 1 >{ value } );
}

bool MoveTecModBus::readRegisters( const uint16_t address, uint16_t* destintation, const int count ) const
{
  if(modbus_read_registers( m_pImpl->ctx, address, count, destintation ) == -1)
  {
    std::cerr << "Failed to read registers: " << modbus_strerror( errno ) << std::endl;
    return false;
  }
  return true;
}

bool MoveTecModBus::writeRegisters( const uint16_t address, const uint16_t* value, const int count ) const
{
  if(modbus_write_registers( m_pImpl->ctx, address, count, value ) == -1)
  {
    std::cerr << "Failed to write register: " << modbus_strerror( errno ) << std::endl;
    return false;
  }
  return true;
}

bool MoveTecModBus::writeRegister( const uint16_t address, const std::array< uint16_t, 1 > value )
{
  return writeRegisters( address, value );
}