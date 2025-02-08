#include "gyrocom.h"

#include <array>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <termios.h>
#include <unistd.h>

namespace
{
  constexpr auto kBUFFER_SIZE = 256;

  auto openPort( const std::string& portName )
  {
    return open( portName.c_str(), O_RDWR | O_NOCTTY | O_SYNC ); // NOLINT(cppcoreguidelines-pro-type-vararg)
  }

  void closePort( int file )
  {
    close( file );
  }

  void writeData( int file, const std::string& data )
  {
    auto bytesWritten = write( file, data.c_str(), data.size() );
    if( bytesWritten == -1 )
    {
      throw std::runtime_error( "Error writing to serial port" );
    }
  }

  std::string readData( int file )
  {
    std::array< char, kBUFFER_SIZE > buffer{};
    buffer.fill( '\0' );
    writeData( file, "P" );

    auto bytesRead = read( file, buffer.data(), sizeof( buffer ) - 1 );
    if( bytesRead == -1 )
    {
      throw std::runtime_error( "Error reading from serial port" );
    }

    return std::string( buffer.data() );
  }

  void configurePort( int file )
  {
    struct termios tty{};
    memset( &tty, 0, sizeof tty );
    if( tcgetattr( file, &tty ) != 0 )
    {
      throw std::runtime_error( "Error getting terminal attributes" );
    }

    cfsetospeed( &tty, B9600 );
    cfsetispeed( &tty, B9600 );

    tty.c_cflag = ( tty.c_cflag & ~CSIZE ) | CS8;
    tty.c_iflag &= ~IGNBRK;
    tty.c_lflag = 0;
    tty.c_oflag = 0;
    tty.c_cc[ VMIN ] = 1;
    tty.c_cc[ VTIME ] = 5; // NOLINT(*-magic-numbers)

    tty.c_iflag &= ~( IXON | IXOFF | IXANY );
    tty.c_cflag |= ( CLOCAL | CREAD );
    tty.c_cflag &= ~( PARENB | PARODD );
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if( tcsetattr( file, TCSANOW, &tty ) != 0 )
    {
      throw std::runtime_error( "Error setting terminal attributes" );
    }
  }

} // namespace

class GyroCom::Impl
{
  public:
    explicit Impl( const std::string& portName )
      : hSerial{ openPort( portName ) }
    {
      if( hSerial == -1 )
      {
        throw std::runtime_error( "Error opening serial port" );
      }
      configurePort( hSerial );
    }
    Impl( const Impl& ) = delete;
    Impl& operator=( const Impl& ) = delete;
    Impl( Impl&& ) = delete;
    Impl& operator=( Impl&& ) = delete;

    ~Impl()
    {
      closePort( hSerial );
    }

    int hSerial;
};

GyroCom::GyroCom( const std::string& portName )
  : m_pImpl{ std::make_unique< Impl >( portName ) }
{
}

GyroCom::~GyroCom() = default;

bool GyroCom::verifyConnection()
{
  std::string data = readData( m_pImpl->hSerial );
  if( data[ 0 ] != 'P' )
  {
    return false;
  }
  std::regex numberRegex( R"(Pitch:\s*(-?\d+))" );
  return std::regex_search( data, numberRegex );
}

double GyroCom::readDouble()
{
  const std::string data = readData( m_pImpl->hSerial );
  if( data.empty() || data[ 0 ] != 'P' )
  {
    return 0.0;
  }

  std::smatch match;
  const std::regex numberRegex(
    R"(Pitch:\s*(-?\d+))" ); // Regular expression to match a number at the end of the string

  if( std::regex_search( data, match, numberRegex ) )
  {
    try
    {
      return std::stod( match.str( 1 ) );
    }
    catch( const std::invalid_argument& e )
    {
      std::cerr << "Invalid argument: " << e.what() << std::endl;
    }
    catch( const std::out_of_range& e )
    {
      std::cerr << "Out of range: " << e.what() << std::endl;
    }
  }
  return 0.0;
}
