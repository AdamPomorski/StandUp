/*
 * bma280.c
 *
 *      Author: PSU - IRE - WUT
 */


#include "bma280.h"
#include <stdio.h>
#include <stdlib.h>


typedef struct MEMS_DATA
{
	char acc_x_raw[2];
	char acc_y_raw[2];
	char acc_z_raw[2];
}mems_data;

signed char height_buf[100];	// Relative hight buffer [cm]
unsigned char height_idx;// Index of last entry in height buffer
unsigned char SPI_TXBUF;
uint8_t buftx[10];
uint8_t  bufrx[10];
unsigned char SPI_RXBUF;
extern SPI_HandleTypeDef hspi1;
extern uint8_t data[10];

//-----------------------------------------------------
//	BASIC BMA280 READ
//-----------------------------------------------------
char bma280_read(char reg){

	uint8_t data2 = 0;
	HAL_GPIO_WritePin(GPIOA,NSS_Pin , GPIO_PIN_RESET);
	//SPI_slave_ON(1);
	//SPI_TXBUF = reg | 0x80;
	buftx[0] = reg | 0x80;
	//SPI_TXBUF = reg | 0x80;
    HAL_SPI_TransmitReceive(&hspi1, buftx, bufrx, 2, 1000);
    data2 = bufrx[1];
	//SPI_slave_OFF(1);
    HAL_GPIO_WritePin(GPIOA,NSS_Pin , GPIO_PIN_SET);
	return data2;
}

//-----------------------------------------------------
//	READ MULTIPLE BYTES
//-----------------------------------------------------
void bma280_readm(char address, uint8_t* data, short bytes){

	int i;

	//SPI_slave_ON(1);
	HAL_GPIO_WritePin(GPIOA,NSS_Pin , GPIO_PIN_RESET);
 	buftx[0] = 0x02 | 0x80;
 	bufrx[0]=0;
 	HAL_SPI_TransmitReceive(&hspi1, buftx, bufrx+1, bytes+1, 1000);
// 	for(i = 0; i<bytes; i++){
//	 	SPI_TXBUF = address | 0x80;
//	 	HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)&SPI_TXBUF, (uint8_t*)&SPI_RXBUF, 1, 1000);
 	for(i=0;i<bytes;i++)
 	{
		data[i] = bufrx[i+1];
	}
 	HAL_GPIO_WritePin(GPIOA,NSS_Pin , GPIO_PIN_SET);

	//SPI_slave_OFF(1);
}

//-----------------------------------------------------
//	BASIC BMA280 WRITE
//-----------------------------------------------------
void bma280_write(char reg, char value){
	HAL_GPIO_WritePin(GPIOA,NSS_Pin , GPIO_PIN_RESET);
	//SPI_slave_ON(1);
	buftx[0] = reg & ~0x80;
	buftx[1] = value;
	HAL_SPI_Transmit(&hspi1,buftx,2,1000);
	//SPI_slave_OFF(1);
	HAL_GPIO_WritePin(GPIOA,NSS_Pin, GPIO_PIN_SET);
}

//-----------------------------------------------------
//	BMA280 INIT
//-----------------------------------------------------
int bma280_init(){
uint8_t bb;

	if(bma280_read(BMA280_ID) == 0xFB){			// ACC chip id check

		bma280_write(BMA280_SOFTRESET, 0xB6);	// ACC softreset
		while(bma280_read(BMA280_SOFTRESET) !=0);// ACC softreset complete
		bma280_write(BMA280_RANGE, 0x05);		// ACC range +-4g
		bma280_write(BMA280_BW, 0x0F);			// ACC bandwidth 31.25 Hz
		bma280_write(BMA280_HIGHBW, 0x00);		// ACC filtering & shadowing ON
//		bma280_write(BMA280_LPM_SEL, 0x80);		// ACC suspend mode
		//bma280_write(BMA280_LPM_CNF, 0x40);		// ACC Low power mode ON
		bma280_write(BMA280_FIFO_CONF_0, 0x20);	//FIFO config_0
		bma280_write(BMA280_FIFO_CONF_1, 0x83);	//FIFO config_1

		//interrupts

		//no motion

		bma280_write(BMA280_INT_MAP_2, 0x08);//maping slo/no motion nt on int2
		bma280_write(BMA280_INT_EN_2, 0x0F);//enable no motion
		bma280_write(BMA280_INT_SRC,0x08);//unfiltered data
		bma280_write(BMA280_INT_LATCH,0x09);//250 micro
		bma280_write(BMA280_INT_5, 0x14);//5s delay
		; //latched


		//any motion
				bma280_write(BMA280_INT_MAP_0, 0x04);/// maping int_slope to int1 port
				bma280_write(BMA280_INT_SRC,0x04);//unfiltered data
				//bma280_write(BMA280_INT_LATCH,0x0F);//latched

				bma280_write(BMA280_INT_EN_0, 0x07); //enabling any-motion detection for all axis
				bma280_write(BMA280_INT_6, 0x0F); // threshhold of slope in any motion
				//bma280_write(BMA280_INT_5, 0x03);// In order to suppress false triggers, the
												 //interrupt is only generated (cleared) if a certain number N of consecutive slope data points is
												 //larger (smaller) than the slope threshold given by (0x28) slope_th.  It is N = (0x27) slope_dur + 1 for (0x27).


		return 1;	// Init complete
	}else{
		return 0;	// ACC ID incorrect
	}
}

//-----------------------------------------------------
//	BMA280 SLEEP
//-----------------------------------------------------
void bma280_sleep(void)
{
	bma280_write(BMA280_LPM_SEL, 0x80);
}

//-----------------------------------------------------
//	BMA280 WAKE UP
//-----------------------------------------------------
void bma280_wakeup(void)
{
	bma280_write(BMA280_LPM_SEL, 0x00);
}

//-----------------------------------------------------
//	READ ACCELEROMETER X,Y,Z DATA
//-----------------------------------------------------
void bma280_read_accl()
{
	mems_data mem;
	int xsign;
	int ysign;
	int zsign;
	bma280_readm(BMA280_X_LSB, data, 8); // LSBx first
	while (!((data[1]&0x01) && (data[3]&0x01) && (data[5]&0x01))){
		bma280_readm(0x02, data, 8);
	}

	if(data[2]&0x80){
		xsign = 0xF000;
	}else{
		xsign = 0x0000;
	}

	if(data[4]&0x80){
		ysign = 0xF000;
	}else{
		ysign = 0x0000;
	}

	if(data[6]&0x80){
		zsign = 0xF000;
	}else{
		zsign = 0x0000;
	}

	// MSB first
//	strcpy(mem.acc_x_raw , (data[1] << 4)&0x0FF0 | (data[0]&0xF0 >> 4) | xsign);	// Get MSB and LSB of accelerometer X raw data
//	strcpy(mem.acc_y_raw, (data[3] << 4)&0x0FF0 | (data[2]&0xF0 >> 4) | ysign);	// Get MSB and LSB of accelerometer Y raw data
//	strcpy(mem.acc_z_raw, (data[5] << 4)&0x0FF0 | (data[4]&0xF0 >> 4) | zsign);	// Get MSB and LSB of accelerometer Z raw data

}
