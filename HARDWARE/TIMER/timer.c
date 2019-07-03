#include "timer.h"
void TIM5_Init(uint32_t prd,uint32_t psc)
{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  ///Ê¹ÄÜTIM3Ê±ÖÓ
//	
//	TIM_TimeBaseInitStructure.TIM_Period = prd; 
//	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  
//	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 
//	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
//	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);
//	
//	
//	TIM_ARRPreloadConfig(TIM5,ENABLE);
//	TIM_Cmd(TIM5,ENABLE); 
}

//get present microsecond.
uint32_t micros(void)
{
//	uint32_t time_now=TIM_GetCounter(TIM5);
//	return time_now;
}

//get present millisecond.
//uint32_t millis() {
//	
//	uint32_t	timer_now = TIM_GetCounter(TIM5);
//	return (uint32_t)(timer_now/1000);
//}

void get_dt_in_micros(u32 *time_now,u32 *time_last,u32 *dt)
{
	*time_now=micros();
	if(*time_now <= *time_last) {
        *dt = *time_now + (0xffffffff - *time_last);
    }
	else{
        *dt = *time_now - *time_last;
	}
	*time_last=*time_now;
}

void get_dt_in_millis(u32 *time_now,u32 *time_last,float *dt)
{
	*time_now=micros();
	if(*time_now <= *time_last) {
        *dt = (float)(*time_now + (0xffffffff - *time_last))/1000.0f;
    }
	else{
        *dt = (float)(*time_now - *time_last)/1000.0f;
	}
	*time_last=*time_now;
}

void get_dt_in_seconds(u32 *time_now,u32 *time_last,float *dt)
{
	*time_now=micros();
	if(*time_now <= *time_last) {
        *dt = (float)(*time_now + (0xffffffff - *time_last))/1000000.0f;
    }
	else{
        *dt = (float)(*time_now - *time_last)/1000000.0f;
	}
	*time_last=*time_now;
}

void calculate_dt_in_micros(u32 time_now,u32 time_last,u32 *dt)
{

	if(time_now <= time_last)
		*dt = time_now + (0xffffffff- time_last);
  else
    *dt = time_now - time_last;
}

void calculate_dt_in_mills(u32 time_now,u32 time_last,float *dt)
{
	if(time_now <= time_last)
		*dt = (float)(time_now + (0xffffffff- time_last))/1000.0f;
  else
    *dt = (float)(time_now - time_last)/1000.0f;
}

void calculate_dt_in_seconds(u32 time_now,u32 time_last,float *dt)
{
	if(time_now <= time_last)
		*dt = (float)(time_now + (0xffffffff- time_last))/1000000.0f;
  else
    *dt = (float)(time_now - time_last)/1000000.0f;
}



