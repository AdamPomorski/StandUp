/*
 * mem.c
 *
 *  Created on: 8 cze 2022
 *      Author: IOT_LAB432_6
 */
#include "mem.h"

extern MemTime memtime_to_save[1000];
extern MemTime memtime_to_equation[1];
extern RTC_TimeTypeDef sTime1;
extern uint32_t events_counter;
void convert_seconds_to_struct(uint32_t seconds_to_struct){
	    uint16_t seconds_counter_substract_minutes;
		uint16_t seconds_counter_substract_seconds;

			memtime_to_save[events_counter].Hours= seconds_to_struct/3600;
			seconds_counter_substract_minutes = seconds_to_struct % 3600;
			memtime_to_save[events_counter].Minutes = seconds_counter_substract_minutes/60;
			seconds_counter_substract_seconds = seconds_counter_substract_minutes % 60;
			memtime_to_save[events_counter].Seconds = seconds_counter_substract_seconds;

}


