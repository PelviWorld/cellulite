// gyrocom.cpp
#include "gyrocom.h"
#include <chrono>
#include <iostream>
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

std::string GyroCom::readData()
{
  char buffer[ 256 ];
  DWORD bytesRead;
  if( ReadFile( hSerial, buffer, sizeof( buffer ) - 1, &bytesRead, nullptr ) )
  {
    buffer[ bytesRead ] = '\0';
    std::cout << "Bytes read: " << bytesRead << std::endl; // Log the number of bytes read
    std::cout << "Data: " << buffer << std::endl;          // Log the data read
    return std::string( buffer );
  }
  else
  {
    std::cerr << "Error reading from serial port" << std::endl;
    throw std::runtime_error( "Error reading from serial port" );
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