#ifndef __BASEFILTER_H
#define __BASEFILTER_H

#include "stm32f4xx_conf.h"

typedef struct 
{
	int16_t FIFO_SIZE;
	int16_t Buf_Pointer;
	int16_t data_count;
	float buf_sum;
	float *FIFO_BUF;
}slide_mean_struct;

float SildeMeanFilter(slide_mean_struct *obj,float newValue);

#endif
