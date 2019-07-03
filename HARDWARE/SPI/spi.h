
#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

#define SPI_OK				0x00
#define SPI_FAILED		0xff
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//SPI ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
 	    	
void SPI1_Init(void);			 //��ʼ��SPI1�� 
uint8_t SPI1_ReadWriteByte(uint8_t TxData,uint8_t *RxData);
void SPI2_Init(void);			 //��ʼ��SPI1��
void SPI2_SetSpeed(u8 SpeedSet); //����SPI1�ٶ�   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI1���߶�дһ���ֽ�
		 
#endif

