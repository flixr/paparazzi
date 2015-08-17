/*
 * Copyright (C) 2015 Felix Ruess <felix.ruess@gmail.com>
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
 * @file subsystems/imu/imu_px4fmu_2.c
 * Driver for the PX4FMU v2.4, MPU6000 and LSM303D gyro/mag via SPI1.
 */

#include "subsystems/imu.h"
#include "subsystems/abi.h"
#include "mcu_periph/spi.h"


/* MPU60x0 gyro/accel internal lowpass frequency */
#if !defined PX4FMU2_LOWPASS_FILTER && !defined  PX4FMU2_SMPLRT_DIV
#if (PERIODIC_FREQUENCY == 60) || (PERIODIC_FREQUENCY == 120)
/* Accelerometer: Bandwidth 44Hz, Delay 4.9ms
 * Gyroscope: Bandwidth 42Hz, Delay 4.8ms sampling 1kHz
 */
#define PX4FMU2_LOWPASS_FILTER MPU60X0_DLPF_42HZ
#define PX4FMU2_SMPLRT_DIV 9
PRINT_CONFIG_MSG("Gyro/Accel output rate is 100Hz at 1kHz internal sampling")
#elif PERIODIC_FREQUENCY == 512
/* Accelerometer: Bandwidth 260Hz, Delay 0ms
 * Gyroscope: Bandwidth 256Hz, Delay 0.98ms sampling 8kHz
 */
#define PX4FMU2_LOWPASS_FILTER MPU60X0_DLPF_256HZ
#define PX4FMU2_SMPLRT_DIV 3
PRINT_CONFIG_MSG("Gyro/Accel output rate is 2kHz at 8kHz internal sampling")
#else
#error Non-default PERIODIC_FREQUENCY: please define PX4FMU2_LOWPASS_FILTER and PX4FMU2_SMPLRT_DIV.
#endif
#endif
PRINT_CONFIG_VAR(PX4FMU2_LOWPASS_FILTER)
PRINT_CONFIG_VAR(PX4FMU2_SMPLRT_DIV)

#ifndef PX4FMU2_GYRO_RANGE
#define PX4FMU2_GYRO_RANGE MPU60X0_GYRO_RANGE_2000
#endif
PRINT_CONFIG_VAR(PX4FMU2_GYRO_RANGE)

#ifndef PX4FMU2_ACCEL_RANGE
#define PX4FMU2_ACCEL_RANGE MPU60X0_ACCEL_RANGE_16G
#endif
PRINT_CONFIG_VAR(PX4FMU2_ACCEL_RANGE)

struct ImuPx4fmu2 imu_px4fmu2;

void imu_impl_init(void)
{
  /* MPU is on spi1 and CS is SLAVE2 */
  mpu60x0_spi_init(&imu_px4fmu2.mpu, &spi1, SPI_SLAVE2);
  // change the default configuration
  imu_px4fmu2.mpu.config.smplrt_div = PX4FMU2_SMPLRT_DIV;
  imu_px4fmu2.mpu.config.dlpf_cfg = PX4FMU2_LOWPASS_FILTER;
  imu_px4fmu2.mpu.config.gyro_range = PX4FMU2_GYRO_RANGE;
  imu_px4fmu2.mpu.config.accel_range = PX4FMU2_ACCEL_RANGE;

  /* read mag of LSM303D on spi1 CS SLAVE1 */
  //lsm303dlhc_init(&imu_px4fmu2.lsm, &spi1, SPI_SLAVE1);
}


void imu_periodic(void)
{
  mpu60x0_spi_periodic(&imu_px4fmu2.mpu);
  //lsm303dlhc_periodic(&imu_px4fmu2.lsm);
}

void imu_px4fmu2_event(void)
{
  uint32_t now_ts = get_sys_time_usec();

  mpu60x0_spi_event(&imu_px4fmu2.mpu);
  if (imu_px4fmu2.mpu.data_available) {
    RATES_ASSIGN(imu.gyro_unscaled,
                 imu_px4fmu2.mpu.data_rates.rates.q,
                 imu_px4fmu2.mpu.data_rates.rates.p,
                 imu_px4fmu2.mpu.data_rates.rates.r);
    VECT3_COPY(imu.accel_unscaled, imu_px4fmu2.mpu.data_accel.vect);
    imu_px4fmu2.mpu.data_available = FALSE;
    imu_scale_gyro(&imu);
    imu_scale_accel(&imu);
    AbiSendMsgIMU_GYRO_INT32(IMU_BOARD_ID, now_ts, &imu.gyro);
    AbiSendMsgIMU_ACCEL_INT32(IMU_BOARD_ID, now_ts, &imu.accel);
  }

#if 0
  lsm303dlhc_event(&imu_px4fmu2.lsm);
  if (imu_px4fmu2.lsm.data_available) {
    VECT3_COPY(imu.accel_unscaled, imu_px4fmu2.lsm.data_mag.vect);
    imu_px4fmu2.lsm.data_available = FALSE;
    imu_scale_accel(&imu);
    AbiSendMsgIMU_ACCEL_INT32(IMU_BOARD_ID, now_ts, &imu.accel);
  }
#endif

}
