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

/** @file subsystems/sensors/baro.c
 *  Common barometric sensor interface
 */


#include "subsystems/sensors/baro.h"

/** Number of values to compute an offset at startup */
#define QFE_NBSAMPLES_AVRG 100

/** Weight for offset IIR filter */
#define QFE_FILTER 7

// TODO set proper scale here
#define PRESSURE_TO_M 3.3

void baro_init(struct Baro *b, baro_cb cb) {
  b->status = BS_UNINITIALIZED;
  b->pressure = 0;
  b->temp = 0;
  b->qfe = 0;
  b->alt_agl = 0.0;
  b->qfe_initialized = FALSE;
  b->valid = FALSE;
  b->filter_enabled = FALSE;
  b->qfe_cnt = QFE_NBSAMPLES_AVRG;
  b->cb = cb;
}

void baro_realign(struct Baro *b) {
  b->qfe_initialized = FALSE;
  b->qfe = 0;
  b->qfe_cnt = QFE_NBSAMPLES_AVRG;
}

void baro_add_measurement(struct Baro *b, uint32_t meas) {
  b->pressure = meas;
  b->valid = TRUE;

  if (!baro.qfe_initialized) {
    /* IIR filter to compute an initial offset */
    b->qfe = (QFE_FILTER * (float)b->qfe + (float)b->absolute) / (QFE_FILTER + 1);
    /* decrease init counter */
    b->offset_cnt--;
    if (b->offset_cnt == 0)
      b->qfe_initialized = TRUE;
  }
  else {

    /* IIR filter to low-pass computed altitude */
    if (b->filter_enabled) {
      // TODO: filter pressure measurements
    }
    /* calculate altitude from pressure measurement */
    b->alt_agl = PRESSURE_TO_M*(float)(b->qfe - b->absolute)
  }

  /* call call-back function if set */
  if (b->cb) b->cb(b);
}
