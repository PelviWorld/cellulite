#pragma once
#include <memory>
#include <string>

class MoveTecModBus
{
  public:
    explicit MoveTecModBus( const std::string& port = "/dev/ttyUSB0" );

    ~MoveTecModBus();

    MoveTecModBus( const MoveTecModBus& ) = delete;

    MoveTecModBus& operator=( const MoveTecModBus& ) = delete;

    MoveTecModBus( MoveTecModBus&& ) = delete;

    MoveTecModBus& operator=( MoveTecModBus&& ) = delete;

    template<std::size_t N>
    bool readRegisters( const uint16_t address, std::array< uint16_t, N >& value )
    {
      return readRegisters( static_cast< uint16_t >( address ), value.data(), N );
    }

    template<std::size_t N>
    bool writeRegisters( const uint16_t address, std::array< uint16_t, N > value )
    {
      return writeRegisters( static_cast< uint16_t >( address ), value.data(), N );
    }

    bool writeRegister( uint16_t address, uint16_t value );

  private:
    bool readRegisters( uint16_t address, uint16_t* destintation, int count = 1 ) const;

    bool writeRegisters( uint16_t address, const uint16_t* value, int count = 1 ) const;

    bool writeRegister( uint16_t address, std::array< uint16_t, 1 > value );

    class Impl;
    std::unique_ptr< Impl > m_pImpl;
};
