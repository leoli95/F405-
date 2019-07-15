#ifndef __USART1_H
#define __USART1_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
#include "stm32f4xx_dma.h"
//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌÐòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßÐí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//Mini STM32¿ª·¢°å
//´®¿Ú1³õÊ¼»¯		   
//ÕýµãÔ­×Ó@ALIENTEK
//¼¼ÊõÂÛÌ³:www.openedv.csom
//ÐÞ¸ÄÈÕÆÚ:2011/6/14
//°æ±¾£ºV1.4
//°æÈ¨ËùÓÐ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ÕýµãÔ­×Ó 2009-2019
//All rights reserved
//********************************************************************************
//V1.3ÐÞ¸ÄËµÃ÷ 
//Ö§³ÖÊÊÓ¦²»Í¬ÆµÂÊÏÂµÄ´®¿Ú²¨ÌØÂÊÉèÖÃ.
//¼ÓÈëÁË¶ÔprintfµÄÖ§³Ö
//Ôö¼ÓÁË´®¿Ú½ÓÊÕÃüÁî¹¦ÄÜ.
//ÐÞÕýÁËprintfµÚÒ»¸ö×Ö·û¶ªÊ§µÄbug
//V1.4ÐÞ¸ÄËµÃ÷
//1,ÐÞ¸Ä´®¿Ú³õÊ¼»¯IOµÄbug
//2,ÐÞ¸ÄÁËUSART_RX_STA,Ê¹µÃ´®¿Ú×î´ó½ÓÊÕ×Ö½ÚÊýÎª2µÄ14´Î·½
//3,Ôö¼ÓÁËUSART_REC_LEN,ÓÃÓÚ¶¨Òå´®¿Ú×î´óÔÊÐí½ÓÊÕµÄ×Ö½ÚÊý(²»´óÓÚ2µÄ14´Î·½)
//4,ÐÞ¸ÄÁËEN_USART1_RXµÄÊ¹ÄÜ·½Ê½
////////////////////////////////////////////////////////////////////////////////// 	
#define USART1_REC_LEN  			50  	//¶¨Òå×î´ó½ÓÊÕ×Ö½ÚÊý 50
#define EN_USART1_RX 			1		//Ê¹ÄÜ£¨1£©/½ûÖ¹£¨0£©´®¿Ú1½ÓÊÕ
typedef struct
{
	uint8_t update;
	uint8_t locked;
	uint8_t rx_length;
	uint8_t tx_length;
	uint16_t tx_size;
	uint16_t rx_size; 
	uint8_t *tx_buf;
	uint8_t *rx_buf;
}usart_rt;
extern uint8_t SBUS_update;
extern u8  USART1_RX_BUF[USART1_REC_LEN]; //½ÓÊÕ»º³å,×î´óUSART_REC_LEN¸ö×Ö½Ú.Ä©×Ö½ÚÎª»»ÐÐ·û 
extern u8 remote[200];
extern u8 remoteflag;
extern u16 fuyi,shengjiang,youmen,fangxiang,channel5,channel6;
extern u16 USART1_RX_STA;         		//½ÓÊÕ×´Ì¬±ê¼Ç	
//Èç¹ûÏë´®¿ÚÖÐ¶Ï½ÓÊÕ£¬Çë²»Òª×¢ÊÍÒÔÏÂºê¶¨Òå
void uart1_init(u32 bound);
#endif
