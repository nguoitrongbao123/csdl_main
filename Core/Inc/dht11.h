#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f4xx_hal.h"


void MX_TIM1_Init(void);
void DHT11_Init(void);
int DHT11_Read_Data(uint8_t *temperature, uint8_t *humidity);
void delay_us(uint16_t us);

#endif
