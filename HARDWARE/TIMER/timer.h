#ifndef __TIME_H
#define __TIME_H
#include "stm32f4xx.h"

void TIM5_Init(uint32_t prd,uint32_t psc);
uint32_t micros(void);
uint32_t millis(void);
void get_dt_in_micros(u32 *time_now,u32 *time_last,u32 *dt);
void get_dt_in_millis(u32 *time_now,u32 *time_last,float *dt);
void get_dt_in_seconds(u32 *time_now,u32 *time_last,float *dt);
void calculate_dt_in_micros(u32 time_now,u32 time_last,u32 *dt);
void calculate_dt_in_mills(u32 time_now,u32 time_last,float *dt);
void calculate_dt_in_seconds(u32 time_now,u32 time_last,float *dt);
	
#endif

