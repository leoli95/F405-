#ifndef __AK8975_H
#define __AK8975_H
#include "stm32f4xx_conf.h"

#define AK8975_ID			 0x48

#define AK8975_WIA     0x00
#define AK8975_INFO		 0x01
#define AK8975_ST1		 0x02
#define AK8975_HXL     0x03
#define AK8975_HXH     0x04
#define AK8975_HYL     0x05
#define AK8975_HYH     0x06
#define AK8975_HZL     0x07
#define AK8975_HZH     0x08
#define AK8975_ST2		 0x09

#define AK8975_CNTL   0x0A
#define AK8975_RSV    0x0B


void AK8975_Init(void);
void AK8975_Get_Magnetism(void);

#endif 
