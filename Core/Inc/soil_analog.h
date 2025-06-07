#ifndef __SOIL_ANALOG_H
#define __SOIL_ANALOG_H

#include "stm32f4xx_hal.h"

#define SOIL_ADC_CHANNEL     ADC_CHANNEL_4   // PA4 = ADC1_IN4

void Soil_Analog_Init(void);
uint16_t Soil_ReadRaw(void);
uint8_t Soil_ReadPercent(void);  // trả về % (0–100)

#endif
