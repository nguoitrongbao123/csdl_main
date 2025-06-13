#ifndef __LIGHT_H
#define __LIGHT_H

#include "stm32f4xx.h"  
#include <stdint.h>


void MX_GPIO_Init(void);

uint16_t LDR_Read(void);

#endif /* __LIGHT_H */
