#include "laingcontroller.h"

#include "ilxccontroller.h"
#include "laingbasicregister.h"
#include "movetecmodbus.h"

#include "lmccontroller.h"
#include "ltccontroller.h"

#include <array>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

namespace
{
  constexpr auto kREGISTER_TIMEOUT = 50;
}

class LaingController::Impl
{
  public:
    explicit Impl( const std::string& device )
      : modBus{ std::make_shared< MoveTecModBus >( device ) }
    {
      readRegister( LAING_REGISTER::FIRMWARE_VERSION, firmwareVersion );
      readRegister( LAING_REGISTER::VALIDITY, validity );
      if( validity == 0 && firmwareVersion == 0 )
      {
        std::cerr << "Device is not valid" << std::endl;
        return;
      }
      readBasicData();
      createController();
    }

    ~Impl() = default;

    Impl( const Impl& ) = delete;

    Impl& operator=( const Impl& ) = delete;

    Impl( Impl&& ) = delete;

    Impl& operator=( Impl&& ) = delete;

    void readBasicData()
    {
      std::array< std::uint16_t, 1 > temp{ 0 };
      modBus->readRegisters( static_cast< std::uint16_t >( LAING_REGISTER::VALIDITY ), temp );
      std::this_thread::sleep_for( std::chrono::milliseconds( kREGISTER_TIMEOUT ) );
      readRegister( LAING_REGISTER::VALIDITY, validity );
      readRegister( LAING_REGISTER::WHO_AM_I, whoAmI );
      readRegister( LAING_REGISTER::FIRMWARE_VERSION, firmwareVersion );
      readRegister( LAING_REGISTER::HARDWARE_VERSION, hardwareVersion );
      readRegister( LAING_REGISTER::SERIAL_NUMBER_LOW, serialNumberLow );
      readRegister( LAING_REGISTER::SERIAL_NUMBER_MID, serialNumberMid );
      readRegister( LAING_REGISTER::SERIAL_NUMBER_HIGH, serialNumberHigh );
      readRegister( LAING_REGISTER::PARAMETER_SET_ID, parameterSetId );
      readRegister( LAING_REGISTER::VENDOR_ID, vendorId );
      readRegister( LAING_REGISTER::VENDOR_PRODUCT_ID, vendorProductId );
      readRegister( LAING_REGISTER::VENDOR_PARAM1, vendorParam1 );
      readRegister( LAING_REGISTER::VENDOR_PARAM2, vendorParam2 );
      readRegister( LAING_REGISTER::INTERNAL_BARCODE_LOW, internalBarcodeLow );
      readRegister( LAING_REGISTER::INTERNAL_BARCODE_HIGH, internalBarcodeHigh );
      readRegister( LAING_REGISTER::PRODUCTION_DATA0, productionData0 );
      readRegister( LAING_REGISTER::PRODUCTION_DATA1, productionData1 );
      readRegister( LAING_REGISTER::M1_CURRENT_CALIBRATION, m1CurrentCalibration );
      readRegister( LAING_REGISTER::M2_CURRENT_CALIBRATION, m2CurrentCalibration );
      readRegister( LAING_REGISTER::M3_CURRENT_CALIBRATION, m3CurrentCalibration );
      readRegister( LAING_REGISTER::M4_CURRENT_CALIBRATION, m4CurrentCalibration );
      readRegister( LAING_REGISTER::GYRO_ORIENTATION, gyroOrientation );
      readRegister( LAING_REGISTER::PARAMETER_FILE_ID, parameterFileId );
      readRegister( LAING_REGISTER::MAX_MOTOR_COUNT, maxMotorCount );
      readRegister( LAING_REGISTER::CONFIG_CHECKSUM_HI, configChecksumHi );
      readRegister( LAING_REGISTER::CONFIG_CHECKSUM_LO, configChecksumLo );
      constexpr auto kWORD_SIZE = 16;
      serialNumber = serialNumberMid << kWORD_SIZE | serialNumberLow;
    }

    void readRegister( const LAING_REGISTER address, std::uint16_t& value ) const
    {
      if( std::array< std::uint16_t, 1 > temp{ 0 };
        modBus->readRegisters( static_cast< std::uint16_t >( address ), temp ) )
      {
        value = temp[ 0 ];
      }
      std::this_thread::sleep_for( std::chrono::milliseconds( kREGISTER_TIMEOUT ) );
    }

    std::shared_ptr< MoveTecModBus > modBus{ nullptr };
    std::unique_ptr< ILxcController > lxcController{ nullptr };
    std::uint16_t validity{ 0 };
    std::uint16_t whoAmI{ 0 };
    std::uint16_t firmwareVersion{ 0 };
    std::uint16_t hardwareVersion{ 0 };
    std::uint16_t serialNumberLow{ 0 };
    std::uint16_t serialNumberMid{ 0 };
    std::uint16_t serialNumberHigh{ 0 };
    std::uint16_t parameterSetId{ 0 };
    std::uint16_t vendorId{ 0 };
    std::uint16_t vendorProductId{ 0 };
    std::uint16_t vendorParam1{ 0 };
    std::uint16_t vendorParam2{ 0 };
    std::uint16_t internalBarcodeLow{ 0 };
    std::uint16_t internalBarcodeHigh{ 0 };
    std::uint16_t productionData0{ 0 };
    std::uint16_t productionData1{ 0 };
    std::uint16_t m1CurrentCalibration{ 0 };
    std::uint16_t m2CurrentCalibration{ 0 };
    std::uint16_t m3CurrentCalibration{ 0 };
    std::uint16_t m4CurrentCalibration{ 0 };
    std::uint16_t gyroOrientation{ 0 };
    std::uint16_t parameterFileId{ 0 };
    std::uint16_t maxMotorCount{ 0 };
    std::uint16_t configChecksumHi{ 0 };
    std::uint16_t configChecksumLo{ 0 };
    int serialNumber{ 0 };

    mutable std::mutex mtx;
    mutable std::condition_variable cv;
    mutable bool isRunning{ false };

    void runInThread( std::function< void() > func )
    {
      std::thread( [ func ]() { func(); } ).detach();
    }

  private:
    void createController()
    {
      if( whoAmI == 0 )
      {
        lxcController = std::make_unique< LtcController >( modBus );
      }
      else if( whoAmI == 1 )
      {
        lxcController = std::make_unique< LmcController >( modBus );
      }
    }
};

LaingController::LaingController()
  : m_pImpl{ nullptr }
{
}

LaingController::LaingController( const std::string& device )
  : m_device( device )
  , m_pImpl{ std::make_unique< Impl >( device ) }
{
}

std::uint16_t LaingController::getTableHeight( const AXIS axis ) const
{
  if( m_pImpl )
  {
    return m_pImpl->lxcController->getTableHeight( axis );
  }
  return 0;
}

LaingController::~LaingController() = default;

void LaingController::setDevice( const std::string& device )
{
  if( m_pImpl )
  {
    m_pImpl.reset();
  }
  m_pImpl = std::make_unique< Impl >( device );
}

void LaingController::moveToUserPosition( const AXIS axis, const USER_POSITION pos ) const
{
  std::unique_lock< std::mutex > lock( m_pImpl->mtx );
  if( m_pImpl->isRunning )
  {
    return; // Discard the call if another operation is still running
  }
  m_pImpl->isRunning = true;
  std::thread( [ this, axis, pos ] {
    m_pImpl->lxcController->moveToUserPosition( axis, pos );
    std::lock_guard< std::mutex > lock( m_pImpl->mtx );
    m_pImpl->isRunning = false;
    m_pImpl->cv.notify_one();
  } )
    .detach();
}

void LaingController::referenceRun( const AXIS axis ) const
{
  std::unique_lock< std::mutex > lock( m_pImpl->mtx );
  if( m_pImpl->isRunning )
  {
    return; // Discard the call if another operation is still running
  }
  m_pImpl->isRunning = true;
  std::thread( [ this, axis ] {
    m_pImpl->lxcController->referenceRun( axis );
    std::lock_guard< std::mutex > lock( m_pImpl->mtx );
    m_pImpl->isRunning = false;
    m_pImpl->cv.notify_one();
  } )
    .detach();
}

int LaingController::getSerialNumber() const
{
  if( m_pImpl )
  {
    return m_pImpl->serialNumber;
  }
  return 0;
}
