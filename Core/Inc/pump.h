#ifndef __PUMP_H
#define __PUMP_H

#include "stm32f4xx_hal.h"

#define PUMP_PORT    GPIOA
#define PUMP_PIN     GPIO_PIN_6  // LED tượng trưng cho bơm

void Pump_Init(void);
void Pump_On(void);
void Pump_Off(void);

#endif
