#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f4xx_hal.h"

void DHT11_Init(void);
int DHT11_Read_Data(uint8_t *temperature, uint8_t *humidity);

#endif
