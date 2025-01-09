// gyrocom.cpp
#include "gyrocom.h"
#include <chrono>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <thread>

GyroCom::GyroCom( const std::string& portName )
{
  hSerial = CreateFile(
    portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr );
  if( hSerial == INVALID_HANDLE_VALUE )
  {
    throw std::runtime_error( "Error opening serial port" );
  }
  configurePort();
}

GyroCom::~GyroCom()
{
  CloseHandle( hSerial );
}
bool GyroCom::verifyConnection()
{
  std::string data = readData();
  if( data[ 0 ] != 'P' )
  {
    return false;
  }
  std::regex numberRegex( R"(Pitch:\s*(-?\d+))" );
  return std::regex_search( data, numberRegex );
}

std::string GyroCom::readData()
{
  writeData( "P" );
  char buffer[ 256 ];
  DWORD bytesRead;
  std::regex numberRegex( R"(Pitch:\s*(-?\d+))" );

  while( true )
  {
    if( ReadFile( hSerial, buffer, sizeof( buffer ) - 1, &bytesRead, nullptr ) )
    {
      buffer[ bytesRead ] = '\0';
      std::string data( buffer );
      return data;
    }
    else
    {
      std::cerr << "Error reading from serial port" << std::endl;
      return "-";
    }
  }
}

void GyroCom::writeData( const std::string& data )
{
  DWORD bytesWritten;
  if( !WriteFile( hSerial, data.c_str(), data.size(), &bytesWritten, nullptr ) )
  {
    std::cerr << "Error writing to serial port" << std::endl;
  }
}

void GyroCom::configurePort()
{
  DCB dcbSerialParams = { 0 };
  dcbSerialParams.DCBlength = sizeof( dcbSerialParams );
  if( !GetCommState( hSerial, &dcbSerialParams ) )
  {
    throw std::runtime_error( "Error getting serial port state" );
  }

  dcbSerialParams.BaudRate = CBR_9600;
  dcbSerialParams.ByteSize = 8;
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

  if( !SetCommState( hSerial, &dcbSerialParams ) )
  {
    throw std::runtime_error( "Error setting serial port state" );
  }

  COMMTIMEOUTS timeouts = { 0 };
  timeouts.ReadIntervalTimeout = 50;
  timeouts.ReadTotalTimeoutConstant = 50;
  timeouts.ReadTotalTimeoutMultiplier = 10;

  if( !SetCommTimeouts( hSerial, &timeouts ) )
  {
    throw std::runtime_error( "Error setting serial port timeouts" );
  }
}