#include "sys.h"
#include "stm32f4xx_conf.h"	
#include "string.h"
#include "stdint.h"
#include "usart1.h"

u8 remote[200];
u8 remoteflag=0;
u16 fuyi,shengjiang,youmen,fangxiang,channel5,channel6;
usart_rt usart1; 
uint8_t		SBUS_DMA_buffer[35];
uint16_t  SBUS_data[16];
uint8_t SBUS_update=0;
#if EN_USART1_RX   
u8 USART1_RX_BUF[USART1_REC_LEN];     
u16 USART1_RX_STA=0;       	

void uart1_init(u32 bound){
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
 
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOA,&GPIO_InitStructure); 

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_2;
	USART_InitStructure.USART_Parity = USART_Parity_Even;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx;	
  USART_Init(USART1, &USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&NVIC_InitStructure);	
  
	USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	USART_Cmd(USART1, ENABLE);
	
	DMA_Config(DMA2_Stream2,DMA_Channel_4, 
						 (uint32_t)&(USART1->DR),
						 (uint32_t)USART1_RX_BUF,
						 DMA_DIR_PeripheralToMemory,
						 USART1_REC_LEN);
						 
	usart1.locked=0;
	usart1.tx_length=0;
	usart1.rx_length=0;
	usart1.tx_size=0;
	usart1.rx_size=USART1_REC_LEN;
	usart1.tx_buf=NULL;
	usart1.rx_buf=USART1_RX_BUF; 
	
}


void USART1_IRQHandler(void)                	
{
	int i;	
	uint8_t	temp = 0;
	uint8_t sbus_count = 0;
	
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET ) {   
		
		temp = USART1->SR;
		temp = USART1->DR;

		DMA_Cmd(DMA2_Stream2, DISABLE);
		DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TCIF2);	
		DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TEIF2);	
		
		temp = USART1_REC_LEN - DMA_GetCurrDataCounter(DMA2_Stream2);	
		if(temp==25)
		{
			for (sbus_count=0; sbus_count<temp; sbus_count++) 
			{	
				SBUS_DMA_buffer[sbus_count] = USART1_RX_BUF[sbus_count];
			}
		
			if (SBUS_DMA_buffer[0]==0x0F &&SBUS_DMA_buffer[24]==0x00 ) {   
				
				SBUS_data[0]= ((SBUS_DMA_buffer[2]&0x07)<<8 )| SBUS_DMA_buffer[1];
				SBUS_data[1]= ((SBUS_DMA_buffer[2]&0xF8)>>3) + ((SBUS_DMA_buffer[3]&0x3F)<<5);
				SBUS_data[2]= ((SBUS_DMA_buffer[3]&0xC0)>>6)+ ((SBUS_DMA_buffer[4]&0xFF)<<2) + ((SBUS_DMA_buffer[5]&0x01)<<10);
				SBUS_data[3]= ((SBUS_DMA_buffer[5]&0xFE)>>1) + ((SBUS_DMA_buffer[6]&0x0F)<<7);
				
				SBUS_data[4]= ((SBUS_DMA_buffer[6]&0xF0)>>4) + ((SBUS_DMA_buffer[7]&0x7F)<<4);
				SBUS_data[5]= ((SBUS_DMA_buffer[7]&0x80)>>7) + ((SBUS_DMA_buffer[8]&0xFF)<<1) + ((SBUS_DMA_buffer[9]&0x03)<<9);
				SBUS_data[6]= ((SBUS_DMA_buffer[9]&0xFC)>>2) + ((SBUS_DMA_buffer[10]&0x1F)<<6) ;
				SBUS_data[7]= ((SBUS_DMA_buffer[10]&0xE0)>>5) + ((SBUS_DMA_buffer[11]&0xFF)<<3);
				
				SBUS_data[8]=  (SBUS_DMA_buffer[12]&0xFF) + ((SBUS_DMA_buffer[13]&0x07)<<8);	
				SBUS_data[9]=  ((SBUS_DMA_buffer[13]&0xF8)>>3) + ((SBUS_DMA_buffer[14]&0x3F)<<5) ;
				SBUS_data[10]= ((SBUS_DMA_buffer[14]&0xC0)>>6) + ((SBUS_DMA_buffer[15]&0xFF)<<2)+ ((SBUS_DMA_buffer[16]&0x01)<<10);
				SBUS_data[11]= ((SBUS_DMA_buffer[16]&0xFE)>>1) + ((SBUS_DMA_buffer[17]&0x0F)<<7) ;
				SBUS_data[12]= ((SBUS_DMA_buffer[17]&0xF0)>>4) + ((SBUS_DMA_buffer[18]&0x7F)<<4);
				SBUS_data[13]= ((SBUS_DMA_buffer[18]&0x80)>>7) + ((SBUS_DMA_buffer[19]&0xFF)<<1) +((SBUS_DMA_buffer[20]&0x03)<<9);
				SBUS_data[14]= ((SBUS_DMA_buffer[20]&0xFC)>>2) + ((SBUS_DMA_buffer[21]&0x1F)<<6);
				SBUS_data[15]= ((SBUS_DMA_buffer[21]&0xE0)>>5) + ((SBUS_DMA_buffer[22]&0xFF)<<3);
				
//				printf("%d",SBUS_DMA_buffer[0]);
				for(i=0;i<16;i++)
				{
					printf("%d",SBUS_data[0]);
				}
//				printf("%d",SBUS_DMA_buffer[24]);
				SBUS_update=1;
			}
		}
		DMA_SetCurrDataCounter(DMA2_Stream2, USART1_REC_LEN);
		DMA_Cmd(DMA2_Stream2, ENABLE);        
    }
} 
#endif	

 




