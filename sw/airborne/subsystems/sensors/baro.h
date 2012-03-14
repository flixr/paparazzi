/*
 * Copyright (C) 2010-2012 The Paparazzi Team
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

/** @file subsystems/sensors/baro.h
 *  Common barometric sensor interface
 */

#ifndef SUBSYSTEMS_SENSORS_BARO_H
#define SUBSYSTEMS_SENSORS_BARO_H

#include <std.h>

enum BaroStatus {
  BS_UNINITIALIZED,
  BS_RUNNING
};

typedef void (*baro_cb) (struct Baro *b);

/** Generic baro structure */
struct Baro {
  uint32_t pressure;      ///< absolute pressure in Pascal
  uint16_t temp;          ///< temperature in Kelvin * 10e-2
  float alt_agl;          ///< altitude AGL in meters (above initial offset)
  uint32_t qfe;           ///< pressure at ground level in Pa (to calculate baro AGL)

  bool_t qfe_initialized; ///< true if QFE was initialized
  bool_t valid;           ///< true if current values are valid
  bool_t filter_enabled;  ///< true if low-pass filtering for agl is enabled
  float r;                ///< altitude measurement noise of this baro

  baro_cb cb;             ///< callback function
  enum BaroStatus status;
};

/** pointer to baro used for the global state */
extern struct Baro* baro;

#include BOARD_CONFIG
#if defined BOARD_HAS_BARO
#include "baro_board.h"
#endif

/** Initialize generic baro structure.
 * @param b Baro struct to initialize
 */
extern void baro_init(struct Baro *b, baro_cb cb);

extern void baro_realign(struct Baro *b);

/**
 * Add a barometer measurement.
 * Also calculates the altitute and applies configured filters.
 * @param b     Baro struct to add the measurement to
 * @param meas  barometric pressure measurement in Pascal
 */
extern void baro_add_measurement(struct Baro *b, uint32_t meas);

#endif /* SUBSYSTEMS_SENSORS_BARO_H */
