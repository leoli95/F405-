#include "MS5611.h"
#include "math.h"
#include "stdbool.h"
#define FIFO_NUM 10

u16 MS5611_C[8]={0,0,0,0,0,0,0,0};
unsigned int D1,D2;
float altitude_baro=0,altitude_baro_ground=0;
float  MS5611_FIFO[FIFO_NUM]; 
uint8_t alt_ground_set=0;
uint8_t baro_available=1;
uint8_t baro_update=0;
slide_mean_struct SMF_baro;

void MS5611_CS_HIGH(void)
{
    GPIO_SetBits(GPIOC, GPIO_Pin_0);
}

void MS5611_CS_LOW(void)
{
    GPIO_ResetBits(GPIOC, GPIO_Pin_0);
}

void MS5611_CS_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
   
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
 
    delay_ms(1);
		
    MS5611_CS_HIGH();
}


u8	MS5611_get_C_value(u8 C_CMD,u16 *value)
{
		u8 byteH=0,byteL=0;
		u8 temp=0;
		u8 spi_err=SPI_OK;
		MS5611_CS_LOW();
		spi_err=SPI1_ReadWriteByte(C_CMD,&temp);
		if(spi_err==SPI_FAILED)
		{
			MS5611_CS_HIGH();
			return SPI_FAILED;
		}
		spi_err=SPI1_ReadWriteByte(0XFF,&byteH);
		if(spi_err==SPI_FAILED)
		{
			MS5611_CS_HIGH();
			return SPI_FAILED;
		}
		spi_err=SPI1_ReadWriteByte(0XFF,&byteL);
		if(spi_err==SPI_FAILED)
		{
			MS5611_CS_HIGH();
			return SPI_FAILED;
		}
		*value=((u16)byteH<<8)|byteL;
		MS5611_CS_HIGH();
		return SPI_OK;
}

static int8_t MS5611_CRC(uint16_t *prom)
{
    int32_t i, j;
    uint32_t res = 0;
    uint8_t crc = prom[7] & 0xF;
		bool blankEeprom = true;
    prom[7] &= 0xFF00;

    for (i = 0; i < 16; i++) {
        if (prom[i >> 1]) {
            blankEeprom = false;
        }
        if (i & 1)
            res ^= ((prom[i >> 1]) & 0x00FF);
        else
            res ^= (prom[i >> 1] >> 8);
        for (j = 8; j > 0; j--) {
            if (res & 0x8000)
                res ^= 0x1800;
            res <<= 1;
        }
    }
    prom[7] |= crc;
    if (!blankEeprom && crc == ((res >> 12) & 0xF))
        return 0;

    return -1;
}

//start convert pressure
void MS5611_convert_D1(void)	
{
	u8 temp=0;
	u8 count=0;
  MS5611_CS_LOW();
	while(SPI1_ReadWriteByte(CONVERT_D1_4096,&temp)!=SPI_OK)
	{
		delay_us(20);
		if(count++==10)
		{
			MS5611_CS_HIGH();
			baro_available=0;
			return;
		}
	}
//  delay_us(40);
  MS5611_CS_HIGH();
}

//start convert temperature
void MS5611_convert_D2(void)
{
	u8 temp=0;
	u8 count=0;
  MS5611_CS_LOW();
	while(SPI1_ReadWriteByte(CONVERT_D2_4096,&temp)!=SPI_OK)
	{
		delay_us(20);
		if(count++==10)
		{
			MS5611_CS_HIGH();
			baro_available=0;
			return;
		}
	}
//	delay_us(40);
  MS5611_CS_HIGH();
}

void MS5611_Init(void)
{
	u8 temp=0;
	u8 spi_err=SPI_OK;
  MS5611_CS_Config();
  MS5611_CS_LOW();
  SPI1_ReadWriteByte(MS5611_RESET_CMD,&temp);//Reset MS5611
  MS5611_CS_HIGH();
	delay_ms(10);
	for(temp=0;temp<8;temp++)
	{
		spi_err=MS5611_get_C_value(MS5611_PROM_READ_CMD+2*temp,&MS5611_C[temp]);
		if(spi_err==SPI_FAILED)
		{
			baro_available=0;
			return;
		}
		delay_ms(10);
	}
	if(MS5611_CRC(MS5611_C)!=0)
	{
		baro_available=0;
		return;
	}
  MS5611_convert_D2();
	delay_ms(10);
	
	SMF_baro.FIFO_SIZE=FIFO_NUM;
	SMF_baro.Buf_Pointer=0;
	SMF_baro.data_count=0;
	SMF_baro.buf_sum=0;
	SMF_baro.FIFO_BUF=&MS5611_FIFO[0];
}

void MS5611_read_ADC(u32 *ADC_value)
{
    u8 byteh,bytem,bytel;//temp_data; 

    MS5611_CS_LOW();
		if(SPI1_ReadWriteByte(MS5611_ADC_READ_CMD,&byteh)!=SPI_OK)
		{
			MS5611_CS_HIGH();
			return;
		}
		if(SPI1_ReadWriteByte(0xff,&byteh)!=SPI_OK)
		{
			MS5611_CS_HIGH();
			return;
		}
		if(SPI1_ReadWriteByte(0xff,&bytem)!=SPI_OK)
		{
			MS5611_CS_HIGH();
			return;
		}
		if(SPI1_ReadWriteByte(0xff,&bytel)!=SPI_OK)
		{
			MS5611_CS_HIGH();
			return;
		}
    delay_us(50);
    MS5611_CS_HIGH();
    *ADC_value = (((uint32_t)byteh)<<16) | (((uint32_t)bytem)<<8) | (bytel);
}

//long long	MS5611_Temperature, MS5611_Pressure, MS5611_Altitude, MS5611_D2, MS5611_D1;
float MS5611_calculate(void)
{
   long long Tmp,DT,P;
   long long OFF1,SENS;
   long long OFF2,SENS2,T2;
   float altitude=0;
	 double tempValue=0;
    DT = D2-((long long)MS5611_C[5]<<8);
		Tmp = 2000 + ((DT* MS5611_C[6])>>23);
		OFF1 =  ((long long)MS5611_C[2]<<16)+(((long long)MS5611_C[4]*DT)>>7);
		SENS =  ((long long)MS5611_C[1]<<15)+(((long long)MS5611_C[3]*DT)>>8);
    if(Tmp <2000)
    {
       T2=DT*DT>>31;					           //T2 always > 0
       OFF2=5*(Tmp-2000)*(Tmp-2000)>>1;  //OFF2 always > 0
       SENS2=5*(Tmp-2000)*(Tmp-2000)>>2;	//SENS2 always > 0
       if(Tmp<-1500)
       {
          OFF2=OFF2+7*(Tmp+1500)*(Tmp+1500);
          SENS2=SENS2+(11*(Tmp+1500)*(Tmp+1500)>>1);
       }
			 Tmp=Tmp-T2;
			 OFF1=OFF1-OFF2;
			 SENS=SENS-SENS2; 
     }
     P = (((long long)D1*SENS>>21)-OFF1)>>15; //P should always greater than zero 
		 tempValue = P/101325.0;
		 tempValue =  pow(tempValue,((double)1.0/5255));
		 altitude = ((44330*(1.0-tempValue))*1000);// m
     return altitude;
}
float ms_dt;
float MS5611_get_height(void)
{  
  static float   altitude=0; 
	static float   altitude_last=0;
	static uint8_t MS5611_count=1;
	static u32     time_now=0,time_last=0;
	static uint8_t first_run=1;
	static u32 convert_dt=0;
	u32 dt=0;
	
	get_dt_in_micros(&time_now,&time_last,&dt);
	convert_dt+=dt;
	if(first_run)
	{
		first_run=0;
		convert_dt=0;
		return altitude_baro_ground;
	}
	if(convert_dt<9040)  //The setting of the convert condition need at least 7.40,typical 8.22,max 9.04.
	{
			return altitude;	
	}
  if(MS5611_count == 1)
  {                                  
		MS5611_read_ADC(&D2);   // On state 1 we read temp         
		MS5611_count++;         
    MS5611_convert_D1();    // Command to start measure pressure. 
		ms_dt=convert_dt;
		convert_dt=0;					  // recyle 
  }
  else
  {   
		if(MS5611_count == 5)
		{                                                
			MS5611_read_ADC(&D1);            
      altitude=MS5611_calculate();
			if(isnan(altitude))
			altitude=altitude_last;
			altitude_last=altitude;
      MS5611_count= 1;        // Start again from state = 1            
      MS5611_convert_D2();    // Command to read temperature 
			ms_dt=convert_dt;
			convert_dt=0;			
    } 
		else
    {       
			MS5611_read_ADC(&D1);                              
			altitude=MS5611_calculate(); 
			if(isnan(altitude))
			altitude=altitude_last;
			altitude_last=altitude;			
			MS5611_count++;   
			MS5611_convert_D1();     // Command to read pressure  
			ms_dt=convert_dt;
			convert_dt=0;
    }
		baro_update=1;
  } 
  return altitude;
} 

void get_alt_baro_ground(float alt_baro)
{
	altitude_baro_ground=SildeMeanFilter(&SMF_baro,alt_baro);
}
