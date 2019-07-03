#include "mpu6050.h"
#include "stm32f4xx_conf.h"
#include "usart.h"


#define filter_high 0.8
#define filter_low  0.2

#define MPU6050_BUF_SIZE 5
float gyro_filter_high=0.5;

FLOAT_XYZ body_acc;		//unit m/s^2.  	
FLOAT_XYZ body_gyro;  //unit rad/s.
uint8_t   DeviceID=0;
extern uint8_t		mpu6050_buffer[14];
int16_t 	Gyro_Offset[3]={0,0,0};
inertial_sensor_data mpu6050_raw,mpu6050;  
slide_mean_struct mpu6050_acc_filter[3];
float MPU6050_ACC_FILTER_BUF[3][MPU6050_BUF_SIZE];
void MPU6050_ACC_Filter_Init(void)
{
	uint8_t i;
	for(i=0;i<3;i++)
	{
		mpu6050_acc_filter[i].Buf_Pointer=0;
		mpu6050_acc_filter[i].buf_sum=0;
		mpu6050_acc_filter[i].data_count=0;
		mpu6050_acc_filter[i].FIFO_SIZE=MPU6050_BUF_SIZE;
		mpu6050_acc_filter[i].FIFO_BUF=MPU6050_ACC_FILTER_BUF[i];
	}
}

void MPU6050_Init(void)
{
	i2cReadBytes(MPUAddress,WHO_AM_I,&DeviceID,1);
	while(DeviceID!=0x68)
	{
		delay_ms(100);
		i2cReadBytes(MPUAddress,WHO_AM_I,&DeviceID,1);
	}
	i2cWriteByte(MPUAddress,PWR_MGMT1,0x00);
	i2cWriteByte(MPUAddress,SMPLRT_DIV,0x07);
	i2cWriteByte(MPUAddress,CONFIG_1,0x06);	
	i2cWriteByte(MPUAddress,GYRO_CONFIG,0x18);	
	i2cWriteByte(MPUAddress,ACCEL_CONFIG,0x18);	//+/-16g
	MPU6050_ACC_Filter_Init();
	Gyro_Offset[0]=352;
	Gyro_Offset[1]=13;
	Gyro_Offset[2]=-1;
}

void Gyro_Get_Offset(void)
{
	Gyro_Offset[0]=mpu6050.gx;
	Gyro_Offset[1]=mpu6050.gy;
	Gyro_Offset[2]=mpu6050.gz;
	mpu6050.gx=0;
	mpu6050.gy=0;
	mpu6050.gz=0;
}
	

float* MPU6050_Get_Motion6()
{
	float a[6];
	//__disable_irq();
	i2cReadBytes(MPUAddress,ACCEL_XOUTH,mpu6050_buffer,14);
	//__enable_irq();
	mpu6050_raw.ax=((((int16_t)mpu6050_buffer[0]) << 8) | mpu6050_buffer[1]) ;
	mpu6050_raw.ay=((((int16_t)mpu6050_buffer[2]) << 8) | mpu6050_buffer[3]) ;
	mpu6050_raw.az=((((int16_t)mpu6050_buffer[4]) << 8) | mpu6050_buffer[5]) ;
	//Ìø¹ýÎÂ¶ÈADC
	mpu6050_raw.gx=((((int16_t)mpu6050_buffer[8]) << 8) | mpu6050_buffer[9])  -Gyro_Offset[0];//75
	mpu6050_raw.gy=((((int16_t)mpu6050_buffer[10]) << 8) | mpu6050_buffer[11])-Gyro_Offset[1];//41
	mpu6050_raw.gz=((((int16_t)mpu6050_buffer[12]) << 8) | mpu6050_buffer[13])-Gyro_Offset[2];//+25
	
//	mpu6050.ax = mpu6050.ax*filter_high+filter_low*mpu6050_raw.ax;
//	mpu6050.ay = mpu6050.ay*filter_high+filter_low*mpu6050_raw.ay;
//	mpu6050.az = mpu6050.az*filter_high+filter_low*mpu6050_raw.az;
	
	mpu6050.ax=SildeMeanFilter(&mpu6050_acc_filter[0],mpu6050_raw.ax);
	mpu6050.ay=SildeMeanFilter(&mpu6050_acc_filter[1],mpu6050_raw.ay);
	mpu6050.az=SildeMeanFilter(&mpu6050_acc_filter[2],mpu6050_raw.az);
	
	mpu6050.gx =mpu6050.gx*gyro_filter_high+mpu6050_raw.gx*(1.0f-gyro_filter_high);
	mpu6050.gy =mpu6050.gy*gyro_filter_high+mpu6050_raw.gy*(1.0f-gyro_filter_high);
	mpu6050.gz =mpu6050.gz*gyro_filter_high+mpu6050_raw.gz*(1.0f-gyro_filter_high);
	
	body_gyro.x=mpu6050.gx*GYRO_SCALE;
	body_gyro.y=mpu6050.gy*GYRO_SCALE;
	body_gyro.z=mpu6050.gz*GYRO_SCALE;
	
	body_acc.x=mpu6050.ax*ACC_SCALE;
	body_acc.y=mpu6050.ay*ACC_SCALE;
	body_acc.z=mpu6050.az*ACC_SCALE;
//	a[0]=mpu6050_buffer[0];
//	a[1]=mpu6050_buffer[1];
//	a[2]=mpu6050_buffer[2];
//	a[3]=mpu6050_buffer[3];
//	a[4]=mpu6050_buffer[4];
//	a[5]=mpu6050_buffer[5];
	
}




