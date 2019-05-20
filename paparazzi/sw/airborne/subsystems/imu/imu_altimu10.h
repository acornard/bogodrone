/*
 * TODO: Add licence
 */

/**
 * @file subsystems/imu/imu_altimu10.h
 *
 * Interface and defaults for the Pololu Altimu10 IMU.
 *
 * 9DoM IMU with LSM6DS33, LIS3MDL and LPS25H, all via I2C.
 */


#ifndef IMU_ALTIMU10_H
#define IMU_ALTIMU10_H

#include "generated/airframe.h"

//#include "peripherals/lsm6ds33.h"
#include "peripherals/lsm6ds33_i2c.h"
#include "subsystems/imu.h"


#ifndef IMU_ALTIMU10_GYRO_RANGE
#define IMU_ALTIMU10_GYRO_RANGE LSM6_G_DEFAULT_FS
#endif

#ifndef IMU_ALTIMU10_ACCEL_RANGE
#define IMU_ALTIMU10_ACCEL_RANGE LSM6_XL_DEFAULT_FS
#endif

extern const float LSM6_GYRO_SENS[4];
extern const int32_t LSM6_GYRO_SENS_FRAC[4][2];

extern const float LSM6_ACCEL_SENS[4];
extern const int32_t LSM6_ACCEL_SENS_FRAC[4][2];



// Set default sensitivity based on range if needed
#if !defined IMU_GYRO_P_SENS & !defined IMU_GYRO_Q_SENS & !defined IMU_GYRO_R_SENS
#define IMU_GYRO_P_SENS LSM6_GYRO_SENS[IMU_ALTIMU10_GYRO_RANGE]
#define IMU_GYRO_P_SENS_NUM LSM6_GYRO_SENS_FRAC[IMU_ALTIMU10_GYRO_RANGE][0]
#define IMU_GYRO_P_SENS_DEN LSM6_GYRO_SENS_FRAC[IMU_ALTIMU10_GYRO_RANGE][1]
#define IMU_GYRO_Q_SENS LSM6_GYRO_SENS[IMU_ALTIMU10_GYRO_RANGE]
#define IMU_GYRO_Q_SENS_NUM LSM6_GYRO_SENS_FRAC[IMU_ALTIMU10_GYRO_RANGE][0]
#define IMU_GYRO_Q_SENS_DEN LSM6_GYRO_SENS_FRAC[IMU_ALTIMU10_GYRO_RANGE][1]
#define IMU_GYRO_R_SENS LSM6_GYRO_SENS[IMU_ALTIMU10_GYRO_RANGE]
#define IMU_GYRO_R_SENS_NUM LSM6_GYRO_SENS_FRAC[IMU_ALTIMU10_GYRO_RANGE][0]
#define IMU_GYRO_R_SENS_DEN LSM6_GYRO_SENS_FRAC[IMU_ALTIMU10_GYRO_RANGE][1]
#endif

// Set default sensitivity based on range if needed
#if !defined IMU_ACCEL_X_SENS & !defined IMU_ACCEL_Y_SENS & !defined IMU_ACCEL_Z_SENS
#define IMU_ACCEL_X_SENS LSM6_ACCEL_SENS[IMU_ALTIMU10_ACCEL_RANGE]
#define IMU_ACCEL_X_SENS_NUM LSM6_ACCEL_SENS_FRAC[IMU_ALTIMU10_ACCEL_RANGE][0]
#define IMU_ACCEL_X_SENS_DEN LSM6_ACCEL_SENS_FRAC[IMU_ALTIMU10_ACCEL_RANGE][1]
#define IMU_ACCEL_Y_SENS LSM6_ACCEL_SENS[IMU_ALTIMU10_ACCEL_RANGE]
#define IMU_ACCEL_Y_SENS_NUM LSM6_ACCEL_SENS_FRAC[IMU_ALTIMU10_ACCEL_RANGE][0]
#define IMU_ACCEL_Y_SENS_DEN LSM6_ACCEL_SENS_FRAC[IMU_ALTIMU10_ACCEL_RANGE][1]
#define IMU_ACCEL_Z_SENS LSM6_ACCEL_SENS[IMU_ALTIMU10_ACCEL_RANGE]
#define IMU_ACCEL_Z_SENS_NUM LSM6_ACCEL_SENS_FRAC[IMU_ALTIMU10_ACCEL_RANGE][0]
#define IMU_ACCEL_Z_SENS_DEN LSM6_ACCEL_SENS_FRAC[IMU_ALTIMU10_ACCEL_RANGE][1]
#endif


struct ImuAltimu10 {
  struct Lsm6_I2c acc_g_lsm6;
  // TODO: Add struct for LIS3 and LPS25H
};

extern struct ImuAltimu10 imu_altimu10;

extern void imu_altimu10_init(void);
extern void imu_altimu10_periodic(void);
extern void imu_altimu10_event(void);

#endif /* IMU_PPZUAV_H */
