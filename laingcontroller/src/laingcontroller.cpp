#include "laingcontroller.h"
#include "movetecmodbus.h"
#include "laingbasicregister.h"

#include <array>

class LaingController::Impl
{
  public:
    explicit Impl( const std::shared_ptr< MoveTecModBus >& modBus_ )
      : modBus{ modBus_ }
    {
      readBasicData();
    }

    ~Impl() = default;

    Impl( const Impl& ) = delete;

    Impl& operator=( const Impl& ) = delete;

    Impl( Impl&& ) = delete;

    Impl& operator=( Impl&& ) = delete;

    void readBasicData()
    {
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

    void readRegister( const LAING_REGISTER address, uint16_t& value ) const
    {
      if(std::array< uint16_t, 1 > temp{ 0 }; modBus->readRegisters( static_cast< uint16_t >( address ), temp ))
      {
        value = temp[ 0 ];
      }
    }

    std::shared_ptr< MoveTecModBus > modBus;
    uint16_t validity{ 0 };
    uint16_t whoAmI{ 0 };
    uint16_t firmwareVersion{ 0 };
    uint16_t hardwareVersion{ 0 };
    uint16_t serialNumberLow{ 0 };
    uint16_t serialNumberMid{ 0 };
    uint16_t serialNumberHigh{ 0 };
    uint16_t parameterSetId{ 0 };
    uint16_t vendorId{ 0 };
    uint16_t vendorProductId{ 0 };
    uint16_t vendorParam1{ 0 };
    uint16_t vendorParam2{ 0 };
    uint16_t internalBarcodeLow{ 0 };
    uint16_t internalBarcodeHigh{ 0 };
    uint16_t productionData0{ 0 };
    uint16_t productionData1{ 0 };
    uint16_t m1CurrentCalibration{ 0 };
    uint16_t m2CurrentCalibration{ 0 };
    uint16_t m3CurrentCalibration{ 0 };
    uint16_t m4CurrentCalibration{ 0 };
    uint16_t gyroOrientation{ 0 };
    uint16_t parameterFileId{ 0 };
    uint16_t maxMotorCount{ 0 };
    uint16_t configChecksumHi{ 0 };
    uint16_t configChecksumLo{ 0 };
    int serialNumber{ 0 };
};

LaingController::LaingController( const std::shared_ptr< MoveTecModBus >& modbus )
  : m_pImpl{ std::make_unique< Impl >( modbus ) }
{
}

LaingController::~LaingController()
{
}
