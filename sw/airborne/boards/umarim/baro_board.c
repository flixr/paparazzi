 /*
 * Copyright (C) 2010 ENAC
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
 *
 */


/** @file boards/umarim/baro_board.c
 *  Driver for the analog Barometer Mpxa6115 using ADC ads1114 (16 bits I2C 860SpS max) from Texas instruments
 *  Navarro & Gorraz & Hattenberger
 */

#include "subsystems/sensors/baro.h"


/** Number of values to compute an offset at startup */
#define OFFSET_NBSAMPLES_AVRG 100

/** Weight for offset IIR filter */
#define OFFSET_FILTER 7

uint16_t offset_cnt;

void baro_init( void ) {
  ads1114_init();

  offset_cnt = OFFSET_NBSAMPLES_AVRG;
}

void baro_periodic( void ) {
  if (BARO_ABS_ADS.data_available) {
    if(baro.status == BS_ALIGNED) {
      baro.altitude = BARO_SENS*(baro.qfe - (float)baro.absolute);
    }
    else if (baro.status == BS_UNINITIALIZED) {
      baro.status = BS_RUNNING;
    }
    else if (baro.status == BS_RUNNING) {
      // IIR filter to compute an initial offset
      baro.qfe = (OFFSET_FILTER * baro.qfe + (float)baro.absolute) / (OFFSET_FILTER + 1);
      // decrease init counter
      --offset_cnt;
      if (offset_cnt == 0) baro.status = BS_ALIGNED;
    }
  }

  // Read the ADC
  ads1114_read(&BARO_ABS_ADS);
}

