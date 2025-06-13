#include "light.h"
#include "ldr_config.h"

uint16_t LDR_Read(void)
{
    __HAL_RCC_ADC1_CLK_ENABLE();

    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    uint16_t value = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);

    return value;
}

void MX_GPIO_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // PA0: analog input
    GPIOA->MODER &= ~(3 << (0 * 2));
    GPIOA->MODER |=  (3 << (0 * 2));
    GPIOA->PUPDR &= ~(3 << (0 * 2));

    // PA5: output (LED)
    GPIOA->MODER &= ~(3 << (5 * 2));
    GPIOA->MODER |=  (1 << (5 * 2));
    GPIOA->OTYPER &= ~(1 << 5);
    GPIOA->OSPEEDR &= ~(3 << (5 * 2));
    GPIOA->PUPDR &= ~(3 << (5 * 2));
}

