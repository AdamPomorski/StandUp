/*
 * mem.h
 *
 *  Created on: 8 cze 2022
 *      Author: IOT_LAB432_6
 */

#ifndef INC_MEM_H_
#define INC_MEM_H_

#include <stdio.h>
#include <stdlib.h>
#include "stm32l0xx_hal.h"

typedef struct {
  uint8_t Hours;

  uint8_t Minutes;

  uint8_t Seconds;
}MemTime;

#endif /* INC_MEM_H_ */
