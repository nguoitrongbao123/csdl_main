#ifndef __LDR_H
#define __LDR_H

#include "stm32f4xx_hal.h"

void LDR_Init(void);
uint16_t LDR_Read(void);

#endif
