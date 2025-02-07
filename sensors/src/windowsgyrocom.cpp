#include "gyrocom.h"

#include <array>
#include <chrono>
#include <iostream>
#include <regex>
#include <windows.h>

namespace
{
  constexpr auto kBUFFER_SIZE = 256;

  void writeData( HANDLE hSerial, const std::string& data )
  {
    DWORD bytesWritten{};
    if( WriteFile( hSerial, data.c_str(), data.size(), &bytesWritten, nullptr ) == 0 )
    {
      std::cerr << "Error writing to serial port" << std::endl;
    }
  }

  std::string readData( HANDLE hSerial )
  {
    std::array< char, kBUFFER_SIZE > buffer{};
    buffer.fill( '\0' );
    DWORD bytesRead{};

    writeData( hSerial, "P" );

    if( ReadFile( hSerial, buffer.data(), sizeof( buffer ) - 1, &bytesRead, nullptr ) == 0 )
    {
      std::cerr << "Error reading from serial port" << std::endl;
    }

    return std::string( buffer.data() );
  };

  DCB getSerialParams( HANDLE hSerial )
  {
    DCB dcbSerialParams = { 0 };
    if( GetCommState( hSerial, &dcbSerialParams ) == 0 )
    {
      throw std::runtime_error( "Error getting serial port state" );
    }

    return dcbSerialParams;
  }

  void dcbSetGyroParameters( DCB& dcbSerialParams )
  {
    constexpr auto kBYTE_SIZE = 8;
    dcbSerialParams.DCBlength = sizeof( dcbSerialParams );
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = kBYTE_SIZE;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    dcbSerialParams.fBinary = TRUE;
    dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;
    dcbSerialParams.fDsrSensitivity = FALSE;
    dcbSerialParams.fTXContinueOnXoff = FALSE;
    dcbSerialParams.fOutX = FALSE;
    dcbSerialParams.fInX = FALSE;
    dcbSerialParams.fErrorChar = FALSE;
    dcbSerialParams.fNull = FALSE;
    dcbSerialParams.fRtsControl = RTS_CONTROL_ENABLE;
    dcbSerialParams.fAbortOnError = FALSE;
    dcbSerialParams.fOutxCtsFlow = FALSE;
    dcbSerialParams.fOutxDsrFlow = FALSE;
  }

  void setCommState( HANDLE hSerial, DCB& dcbSerialParams )
  {
    if( SetCommState( hSerial, &dcbSerialParams ) == 0 )
    {
      throw std::runtime_error( "Error setting serial port state" );
    }
  }

  COMMTIMEOUTS gyroTimeouts()
  {
    constexpr auto kTIMEOUT = 50;
    constexpr auto kMULTIPLIER = 10;
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = kTIMEOUT;
    timeouts.ReadTotalTimeoutConstant = kTIMEOUT;
    timeouts.ReadTotalTimeoutMultiplier = kMULTIPLIER;
    return timeouts;
  }

  void setCommTimeouts( HANDLE hSerial, COMMTIMEOUTS& timeouts )
  {
    if( SetCommTimeouts( hSerial, &timeouts ) == 0 )
    {
      throw std::runtime_error( "Error setting serial port timeouts" );
    }
  }

  void configurePort( HANDLE hSerial )
  {
    auto dcbSerialParams = getSerialParams( hSerial );
    auto timeouts = gyroTimeouts();

    dcbSetGyroParameters( dcbSerialParams );
    setCommState( hSerial, dcbSerialParams );
    setCommTimeouts( hSerial, timeouts );
  }
} // namespace

class GyroCom::Impl
{
  public:
    explicit Impl( const std::string& portName )
      : hSerial{ CreateFile(
          portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr ) }
    {
      if( hSerial == INVALID_HANDLE_VALUE )
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
      CloseHandle( hSerial );
    }

    HANDLE hSerial;
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
