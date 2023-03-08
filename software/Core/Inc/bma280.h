/*
 * bma280.h
 *
 *  Created on: 13 mar 2015
 *      Author: Bere
 */

#ifndef BMA280_H_
#define BMA280_H_

#include <stdint.h>
#include <string.h>
#include "stm32l0xx_hal.h"
#include "main.h"

#define BMA280_ID       	0x00   // R
#define BMA280_X_LSB       	0x02   // R
#define BMA280_X_MSB       	0x03   // R
#define BMA280_Y_LSB       	0x04   // R
#define BMA280_Y_MSB       	0x05   // R
#define BMA280_Z_LSB       	0X06   // R
#define BMA280_Z_MSB       	0x07   // R
#define BMA280_TEMPR        0x08
#define BMA280_INT_STAT_0   0x09
#define BMA280_INT_STAT_1	0x0A
#define BMA280_INT_STAT_2   0x0B	
#define BMA280_INT_STAT_3	0x0C
#define BMA280_FIFO_STAT	0x0E
#define BMA280_RANGE       	0x0F   // R/W +-8G => 0x08
#define BMA280_BW 	     	0x10   // R/W 0x0A => 31.25 Hz
#define BMA280_LPM_SEL		0x11   // R/W
#define BMA280_LPM_CNF		0x12   // R/W 
#define BMA280_HIGHBW     	0x13   // R/W 0x00 => filtering & shadowing on
#define BMA280_SOFTRESET 	0x14   // R/W 0xB6
#define BMA280_INT_EN_0     0x16
#define BMA280_INT_EN_1		0x17
#define BMA280_INT_EN_2     0x18
#define BMA280_INT_MAP_0	0x19
#define BMA280_INT_MAP_1	0x1A
#define BMA280_INT_MAP_2	0x1B
#define BMA280_INT_SRC		0x1E
#define BMA280_INT_OUT_CTRL	0x20
#define BMA280_INT_LATCH	0x21
#define BMA280_INT_0 		0x22
#define BMA280_INT_1 		0x23
#define BMA280_INT_2 		0x24
#define BMA280_INT_3 		0x25
#define BMA280_INT_4 		0x26
#define BMA280_INT_5        0x27
#define BMA280_INT_6        0x28
#define BMA280_INT_7        0x29
#define BMA280_INT_8        0x2A
#define BMA280_INT_9        0x2B
#define BMA280_INT_A        0x2C
#define BMA280_INT_B        0x2D
#define BMA280_INT_C        0x2E
#define BMA280_INT_D        0x2F
#define BMA280_FIFO_CONF_0  0x30
#define BMA280_SELF_TEST    0x32
#define BMA280_NVM_CTRL     0x33
#define BMA280_SPI3_WDT     0x34
#define BMA280_OFC_CTRL     0x36
#define BMA280_OFC_SETTING  0x37
#define BMA280_OFFSET_X     0x38
#define BMA280_OFFSET_Y     0x39
#define BMA280_OFFSET_Z     0x3A
#define BMA280_GP0          0x3B
#define BMA280_GP1          0x3C
#define BMA280_FIFO_CONF_1  0x3E
#define BMA280_FIFO_DATA    0x3F

int bma280_init(void);

void bma280_sleep(void);
void bma280_wakeup(void);

char bma280_read(char reg);
void bma280_read_accl(void);
void bma280_readm(char address, uint8_t* data, short bytes);
void bma280_readFIFO(void);
void bma280_write(char address, char value);

#endif /* BMA280_H_ */
