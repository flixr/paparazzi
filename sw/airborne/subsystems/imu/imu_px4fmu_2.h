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
 * @file subsystems/imu/imu_px4fmu_2.h
 * Driver for the PX4FMU v2.4, MPU6000 and LSM303D gyro/mag via SPI1.
 */

#ifndef IMU_PX4FMU_2_H
#define IMU_PX4FMU_2_H

#include "std.h"
#include "generated/airframe.h"
#include "subsystems/imu.h"

#include "subsystems/imu/imu_mpu60x0_defaults.h"
#include "peripherals/mpu60x0_spi.h"
#include "peripherals/lsm303dlhc.h"

struct ImuPx4fmu2 {
  struct Mpu60x0_Spi mpu;
  struct Lsm303dlhc lsm;
};

extern struct ImuPx4fmu2 imu_px4fmu2;

extern void imu_px4fmu2_event(void);

#define ImuEvent imu_px4fmu2_event

#endif /* IMU_PX4FMU_H */
