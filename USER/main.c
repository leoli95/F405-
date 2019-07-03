#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "spi.h"
#include "key.h" 
#include "24l01.h"
#include "stm32f4xx_conf.h"
#include "timer.h"
float gaodu;
//ALIENTEK 探索者STM32F407开发板 实验33
//NRF24L02无线通信实验-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK
//要写入到W25Q16的字符串数组
const u8 TEXT_Buffer[]={"Explorer STM32F4 SPI TEST"};
#define SIZE sizeof(TEXT_Buffer)	 
uint8_t		mpu6050_buffer[14];
uint8_t ak8975_buffer[6];
int main(void)
{ 		 
	float *a;
	int i;
	u8 tmp_buf[33];	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(19200);	//初始化串口波特率为115200
//	LED_Init();					//初始化LED 
// 	LCD_Init();					//LCD初始化  
	i2cdevInit();
// 	KEY_Init();					//按键初始化
	MPU6050_Init();
 	NRF24L01_Init();    		//初始化NRF24L01 
	SPI1_Init();
	MS5611_Init();
	TIM5_Init(41,5000);
	AK8975_Init();
	
//	LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");	
//	LCD_ShowString(30,70,200,16,16,"NRF24L01 TEST");	
//	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
//	LCD_ShowString(30,110,200,16,16,"2014/5/9");
//	printf("sdsdasda\r\n");
//	printf("sdsdasda\r\n");
//	printf("sdsdasda\r\n");
//	printf("sdsdasda\r\n");
//	printf("sdsdasda\r\n");
	while(NRF24L01_Check()); 
				
				tmp_buf[0]=0x06;
				tmp_buf[1]=1;
				tmp_buf[2]='2';
				tmp_buf[3]='3';
				tmp_buf[4]='4';
				tmp_buf[5]='5';
				tmp_buf[6]='6';
				NRF24L01_TX_Mode();
				NRF24L01_TxPacket(tmp_buf);
				NRF24L01_Write_Reg(FLUSH_TX,0xff);
		while(1){		
   			AK8975_Get_Magnetism();

				gaodu=MS5611_get_height();
			

				a=MPU6050_Get_Motion6();

				tmp_buf[0]=0x13;
			
				tmp_buf[1]=mpu6050_buffer[0];
				tmp_buf[2]=mpu6050_buffer[1];
				tmp_buf[3]=mpu6050_buffer[2];
				tmp_buf[4]=mpu6050_buffer[3];
				tmp_buf[5]=mpu6050_buffer[4];
				tmp_buf[6]=mpu6050_buffer[5];
			
				tmp_buf[7]=mpu6050_buffer[8];
				tmp_buf[8]=mpu6050_buffer[9];
				tmp_buf[9]=mpu6050_buffer[10];
				tmp_buf[10]=mpu6050_buffer[11];
				tmp_buf[11]=mpu6050_buffer[12];
				tmp_buf[12]=mpu6050_buffer[13];
				
				tmp_buf[13]=ak8975_buffer[1];
				tmp_buf[14]=ak8975_buffer[0];
				tmp_buf[15]=ak8975_buffer[3];
				tmp_buf[16]=ak8975_buffer[2];
				tmp_buf[17]=ak8975_buffer[5];
				tmp_buf[18]=ak8975_buffer[4];
				
				tmp_buf[19]=gaodu;
				
				
				
				
				
				
				NRF24L01_TxPacket(tmp_buf);

				NRF24L01_Write_Reg(FLUSH_TX,0xff);

			
			    
				

	} 
}

			   
		

