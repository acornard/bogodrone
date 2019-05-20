/*
 * Copyright (C) 2019 Alexis Cornard <alexiscornard@gmail.com>
 *
 * This file is part of paparazzi.
 *
 * paparazzi is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * paparazzi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with paparazzi; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/**
 * @file subsystems/imu/imu_altimu10.h
 *
 * Driver for the Plolu Altimu10 IMU.
 *
 * 9DoM IMU with LSM6DS33, LIS3MDL and LPS25H, all via I2C.
 */

#include "subsystems/imu.h"
#include "subsystems/abi.h" 
#include "mcu_periph/i2c.h"
#include "imu_altimu10.h"


/* i2c default */
#ifndef IMU_ALTIMU10_I2C_DEV
#define IMU_ALTIMU10_I2C_DEV i2c2
#endif

#ifndef IMU_ALTIMU10_I2C_ADDR
#define IMU_ALTIMU10_I2C_ADDR LSM6_ADDR
#endif
 

struct ImuAltimu10 imu_altimu10;

const float LSM6_ACCEL_SENS[4] = {
  LSM6_ACCEL_SENS_2G,
  LSM6_ACCEL_SENS_4G,
  LSM6_ACCEL_SENS_8G,
  LSM6_ACCEL_SENS_16G
};

const int32_t LSM6_ACCEL_SENS_FRAC[4][2] = {
  { LSM6_ACCEL_SENS_2G_NUM, LSM6_ACCEL_SENS_2G_DEN },
  { LSM6_ACCEL_SENS_4G_NUM, LSM6_ACCEL_SENS_4G_DEN },
  { LSM6_ACCEL_SENS_8G_NUM, LSM6_ACCEL_SENS_8G_DEN },
  { LSM6_ACCEL_SENS_16G_NUM, LSM6_ACCEL_SENS_16G_DEN }
};


const float LSM6_GYRO_SENS[4] = {
  LSM6_GYRO_SENS_245,
  LSM6_GYRO_SENS_500,
  LSM6_GYRO_SENS_1000,
  LSM6_GYRO_SENS_2000
};

const int32_t LSM6_GYRO_SENS_FRAC[4][2] = {
  { LSM6_GYRO_SENS_245_NUM, LSM6_GYRO_SENS_245_DEN },
  { LSM6_GYRO_SENS_500_NUM, LSM6_GYRO_SENS_500_DEN },
  { LSM6_GYRO_SENS_1000_NUM, LSM6_GYRO_SENS_1000_DEN },
  { LSM6_GYRO_SENS_2000_NUM, LSM6_GYRO_SENS_2000_DEN }
};


void imu_altimu10_init(void)
{
  /* Set accel and gyro configuration */
  lsm6_i2c_init(&imu_altimu10.acc_g_lsm6, &(IMU_ALTIMU10_I2C_DEV), IMU_ALTIMU10_I2C_ADDR);

  // TODO: Add other device configuration
}


void imu_altimu10_periodic(void)
{
  lsm6_i2c_periodic(&imu_altimu10.acc_g_lsm6);
}

void imu_altimu10_event(void)
{ 
  uint32_t now_ts = get_sys_time_usec();

  lsm6_i2c_event(&imu_altimu10.acc_g_lsm6);
  if (imu_altimu10.acc_g_lsm6.data_available) {
    imu.accel_unscaled.x = -imu_altimu10.acc_g_lsm6.data_xl.vect.x;
    imu.accel_unscaled.y =  imu_altimu10.acc_g_lsm6.data_xl.vect.y;
    imu.accel_unscaled.z = -imu_altimu10.acc_g_lsm6.data_xl.vect.z;

    imu.gyro_unscaled.p = -imu_altimu10.acc_g_lsm6.data_g.rates.p;
    imu.gyro_unscaled.q =  imu_altimu10.acc_g_lsm6.data_g.rates.q;
    imu.gyro_unscaled.r = -imu_altimu10.acc_g_lsm6.data_g.rates.r;

    imu_altimu10.acc_g_lsm6.data_available = false;
    imu_scale_accel(&imu);
    imu_scale_gyro(&imu);
    AbiSendMsgIMU_ACCEL_INT32(IMU_ALTIMU10_ID, now_ts, &imu.accel);
    AbiSendMsgIMU_GYRO_INT32(IMU_ALTIMU10_ID, now_ts, &imu.gyro);
  }

  // TODO: Add event for other device
}
