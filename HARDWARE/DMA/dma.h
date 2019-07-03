#ifndef __DMA_H
#define	__DMA_H	   
#include "sys.h"
#include "stm32f4xx_dma.h"
void DMA_Config(DMA_Stream_TypeDef *DMA_Streamx,uint32_t chx,uint32_t par,uint32_t mar,uint32_t dir,u16 ndtr);
void DMA_Transfer_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr);	//Ê¹ÄÜÒ»´ÎDMA´«Êä		   
#endif
