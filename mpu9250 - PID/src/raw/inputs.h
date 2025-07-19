#define MPU9250_IMU_ADDRESS 0x68
#define MPU9250_MAG_ADDRESS 0x0C

#define GYRO_FULL_SCALE_250_DPS  0x00
#define GYRO_FULL_SCALE_500_DPS  0x08
#define GYRO_FULL_SCALE_1000_DPS 0x10
#define GYRO_FULL_SCALE_2000_DPS 0x18

#define ACC_FULL_SCALE_2G  0x00
#define ACC_FULL_SCALE_4G  0x08
#define ACC_FULL_SCALE_8G  0x10
#define ACC_FULL_SCALE_16G 0x18

#define TEMPERATURE_OFFSET 21 // As defined in documentation

#define INTERVAL_MS_PRINT 1000

#define G 9.80665

#define target_angle 0 // target bearing

struct gyroscope_raw {
  int16_t x, y, z;
} gyroscope;

struct accelerometer_raw {
  int16_t x, y, z;
} accelerometer;

struct magnetometer_raw {
  int16_t x, y, z;

  struct {
    int8_t x, y, z;
  } adjustment;
} magnetometer;

struct temperature_raw {
  int16_t value;
} temperature;

struct {
  struct {
    float x, y, z;
  } accelerometer, gyroscope, magnetometer;

  float temperature;
} normalized;
