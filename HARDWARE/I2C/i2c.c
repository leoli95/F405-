#include "delay.h"
#include "i2c.h"

uint32_t i2c_soft_err=0;

void i2cdevInit()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//ÏÈÊ¹ÄÜÍâÉèIO PORTCÊ±ÖÓ 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	
	   
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT ;  //ÆÕÍ¨Êä³öÄ£Ê½
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//ÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	IIC_SCL=1;
	IIC_SDA=1;
}
//²úÉúIICÆðÊ¼ÐÅºÅ
void IIC_Start(void)
{
	SDA_OUT();     //sdaÏßÊä³ö
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//Ç¯×¡I2C×ÜÏß£¬×¼±¸·¢ËÍ»ò½ÓÊÕÊý¾Ý 
}	  
//²úÉúIICÍ£Ö¹ÐÅºÅ
void IIC_Stop(void)
{
	SDA_OUT();//sdaÏßÊä³ö
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//·¢ËÍI2C×ÜÏß½áÊøÐÅºÅ
	delay_us(4);							   	
}
//µÈ´ýÓ¦´ðÐÅºÅµ½À´
//·µ»ØÖµ£º1£¬½ÓÊÕÓ¦´ðÊ§°Ü
//        0£¬½ÓÊÕÓ¦´ð³É¹¦
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDAÉèÖÃÎªÊäÈë  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//Ê±ÖÓÊä³ö0 	   
	return 0;  
} 
//²úÉúACKÓ¦´ð
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//²»²úÉúACKÓ¦´ð		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC·¢ËÍÒ»¸ö×Ö½Ú
//·µ»Ø´Ó»úÓÐÎÞÓ¦´ð
//1£¬ÓÐÓ¦´ð
//0£¬ÎÞÓ¦´ð			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//À­µÍÊ±ÖÓ¿ªÊ¼Êý¾Ý´«Êä
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //¶ÔTEA5767ÕâÈý¸öÑÓÊ±¶¼ÊÇ±ØÐëµÄ
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//¶Á1¸ö×Ö½Ú£¬ack=1Ê±£¬·¢ËÍACK£¬ack=0£¬·¢ËÍnACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDAÉèÖÃÎªÊäÈë
  for(i=0;i<8;i++ )
	{
    IIC_SCL=0; 
    delay_us(2);
		IIC_SCL=1;
    receive<<=1;
    if(READ_SDA)receive++;   
		delay_us(1); 
	}					 
  if (!ack)
    IIC_NAck();//·¢ËÍnACK
  else
    IIC_Ack(); //·¢ËÍACK   
  return receive;
}
void i2cWriteByte(u8 DevID,u8 Addr,u8 Dat)
{
	IIC_Start();
	IIC_Send_Byte(DevID << 1| 0);								//·¢ËÍÉè±¸µØÖ·ºÍÐ´ÐÅºÅ
	IIC_Wait_Ack();
	IIC_Send_Byte(Addr);
	IIC_Wait_Ack();
	IIC_Send_Byte(Dat);
	IIC_Wait_Ack();
	IIC_Stop();
	delay_ms(10);
}
void i2cReadBytes(u8 DevID,u8 Addr,u8 *Pbuf,u8 Num)
{
	u8 i;
	u8 err;
	IIC_Start();
	IIC_Send_Byte(DevID << 1 | 0);	//·¢ËÍÉè±¸µØÖ·ºÍÐ´ÐÅºÅ
	err=IIC_Wait_Ack();
	if(err) 
	{
		i2c_soft_err++;
		return;
	}
	IIC_Send_Byte(Addr);
	err=IIC_Wait_Ack();
	if(err)
	{
		i2c_soft_err++;
		return;
	}
	IIC_Start();
	IIC_Send_Byte(DevID << 1 | 1);	//·¢ËÍÉè±¸µØÖ·ºÍ¶ÁÐÅºÅ
	err=IIC_Wait_Ack();
	if(err) 
	{
		i2c_soft_err++;
		return;
	}
	for(i = 0;i < (Num - 1);i ++)
	{
		Pbuf[i] = IIC_Read_Byte(1);
	}
	Pbuf[i] = IIC_Read_Byte(0);
	IIC_Stop();
}











