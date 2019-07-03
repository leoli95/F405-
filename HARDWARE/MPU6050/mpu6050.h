#ifndef __MPU6050_H
#define __MPU6050_H


#include "stm32f4xx_conf.h"
/* MPU6050 Register Map */
#define MPUAddress    					0x68	//MPU6050 7bit Address
#define	SMPLRT_DIV							0x19	
#define	CONFIG_1								0x1A
#define	GYRO_CONFIG							0x1b
#define ACCEL_CONFIG						0x1c
#define ACCEL_XOUTH							0x3b
#define ACCEL_XOUTL							0x3c
#define ACCEL_YOUTH							0x3d
#define ACCEL_YOUTL							0x3e
#define ACCEL_ZOUTH							0x3f
#define ACCEL_ZOUTL							0x40
#define	TEMP_OUTH							  0x41
#define	TEMP_OUTL							  0x42
#define GYRO_XOUTH							0x43
#define GYRO_XOUTL							0x44
#define GYRO_YOUTH							0x45
#define GYRO_YOUTL							0x46
#define GYRO_ZOUTH							0x47
#define GYRO_ZOUTL							0x48
#define PWR_MGMT1								0x6b
#define	PWR_MGMT2								0x6c
#define WHO_AM_I								0x75
#define GYRO_SCALE						0.0010653	
#define ACC_SCALE						  0.0048828    //10/2048

typedef struct 
{
	int16_t ax;
	int16_t ay;
	int16_t az;
	int16_t gx;
	int16_t gy;
	int16_t gz;
//	int16_t mx;
//	int16_t my;
//	int16_t mz;
}inertial_sensor_data;
typedef struct
{
	float x;
	float y;
	float z;
}FLOAT_XYZ;
extern float gyro_filter_high;
extern void MPU6050_Init(void);
extern float* MPU6050_Get_Motion6(void);

#endif
