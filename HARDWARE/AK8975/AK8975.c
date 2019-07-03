#include "Ak8975.h"
#include "stdio.h"

#define ak_FC			0.5f
typedef struct
{
	int16_t X;
	int16_t Y;
	int16_t Z;
}S_INT16_XYZ;

typedef struct
{
	float x;
	float y;
	float z;
}MAGN_DATA;

extern uint8_t ak8975_buffer[6];
S_INT16_XYZ AK8975_LAST={0,0,0};
MAGN_DATA ak8975={0,0,0};

void AK8975_CS_HIGH(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_10);
}

void AK8975_CS_LOW(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
}

void AK8975_Pin_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_0);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_0);
	GPIO_SetBits(GPIOB, GPIO_Pin_10);
}

void AK8975_Write_Reg(unsigned char addr,unsigned char value)
{
	u8 temp=0;
	AK8975_CS_LOW();
	SPI1_ReadWriteByte(addr,&temp);
	SPI1_ReadWriteByte(value,&temp);
	AK8975_CS_HIGH();
}

void AK8975_Read_Regs(unsigned char *buf,uint8_t addr,uint8_t num)
{
	uint8_t i;
	u8 temp=0;
	AK8975_CS_LOW();
	SPI1_ReadWriteByte(addr|=0x80,&temp);
	for(i=0;i<num;i++)
	{
		SPI1_ReadWriteByte(0xFF,buf+i);
	}
	AK8975_CS_HIGH();
}


void AK8975_Init(void)
{
	u8 id=0;
	AK8975_Pin_Init();
	delay_ms(10);
	AK8975_Read_Regs(&id,AK8975_WIA,1);
	while(id!=AK8975_ID)
	{
		delay_ms(250);
		AK8975_Read_Regs(&id,AK8975_WIA,1);
	}
	AK8975_Write_Reg(AK8975_CNTL,0x01);
}

int16_t Ak_Xo=-240,Ak_Yo=-31,Ak_Zo=93;
void AK8975_Get_Magnetism(void)
{
	u8 DRDY=0;
	AK8975_Read_Regs(&DRDY,AK8975_ST1,1);
	if(!DRDY)
		return;
	AK8975_Read_Regs(ak8975_buffer,AK8975_HXL,6);

	AK8975_Write_Reg(AK8975_CNTL,0x01);
	
	AK8975_LAST.X=((((int16_t)ak8975_buffer[1]) << 8) | ak8975_buffer[0])-Ak_Xo;
	AK8975_LAST.Y=((((int16_t)ak8975_buffer[3]) << 8) | ak8975_buffer[2])-Ak_Yo;
	AK8975_LAST.Z=((((int16_t)ak8975_buffer[5]) << 8) | ak8975_buffer[4])-Ak_Zo;
	
	ak8975.x=ak_FC*ak8975.x+(1.0f-ak_FC)*AK8975_LAST.X;
	ak8975.y=ak_FC*ak8975.y+(1.0f-ak_FC)*AK8975_LAST.Y;
	ak8975.z=ak_FC*ak8975.z+(1.0f-ak_FC)*AK8975_LAST.Z;
	
//	printf("\r\n%d,%d,%d;\r\n",AK8975_LAST.X,AK8975_LAST.Y,AK8975_LAST.Z);
//	delay_ms(1000);
}
