#include "BaseFilter.h"

float SildeMeanFilter(slide_mean_struct *obj,float newValue)
{
	int16_t pointer=obj->Buf_Pointer;
	float outValue;
	
	obj->buf_sum +=newValue;
	obj->data_count++;
	if(obj->data_count>obj->FIFO_SIZE)
	{
		obj->data_count=obj->FIFO_SIZE;
		obj->buf_sum-=obj->FIFO_BUF[pointer];
	}
	
	obj->FIFO_BUF[pointer]=newValue;
	obj->Buf_Pointer++;	
	if(obj->Buf_Pointer==obj->FIFO_SIZE)
		obj->Buf_Pointer=0;
	
	outValue=obj->buf_sum/obj->data_count;
	
	return outValue;
}
