#ifndef __MS5611_H
#define __MS5611_H
#include "stm32f4xx_conf.h"

// =============================================================================
#define  MS5611_ADDR           0xEE

#define  MS5611_RESET_CMD      0x1E
#define  MS5611_PROM_READ_CMD  0xA0
#define  MS5611_C1_READ_CMD    0xA2
#define  MS5611_C2_READ_CMD    0xA4
#define  MS5611_C3_READ_CMD    0xA6
#define  MS5611_C4_READ_CMD    0xA8
#define  MS5611_C5_READ_CMD    0xAA
#define  MS5611_C6_READ_CMD    0xAC
#define  MS5611_ADC_READ_CMD   0x00

#define  CONVERT_D1_4096       0x48
#define  CONVERT_D1_2048       0x46
#define  CONVERT_D1_1024       0x44
#define  CONVERT_D1_512        0x42
#define  CONVERT_D1_256        0x40

#define  CONVERT_D2_4096       0x58
#define  CONVERT_D2_2048       0x56
#define  CONVERT_D2_1024       0x54
#define  CONVERT_D2_512        0x52
#define  CONVERT_D2_256        0x50

// =============================================================================
extern uint8_t baro_available;
extern uint8_t baro_update;
extern uint8_t alt_ground_set;
// variables
extern float altitude_baro,altitude_baro_ground;
//function
void MS5611_Init(void);
float MS5611_get_height(void);
void get_alt_baro_ground(float alt_baro);
#endif
