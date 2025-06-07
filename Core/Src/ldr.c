#include "ldr.h"

ADC_HandleTypeDef hadc1;
void MX_ADC1_Init(void)
{
    __HAL_RCC_ADC1_CLK_ENABLE();

    hadc1.Instance = ADC1;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;

    HAL_ADC_Init(&hadc1);
}

uint16_t LDR_Read(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_0; // PA0
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
    // 1. Enable GPIOA clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // === PA0: Analog input (LDR) ===
    // MODER0 = 11 (analog mode)
    GPIOA->MODER &= ~(3 << (0 * 2));      // Clear MODER0
    GPIOA->MODER |=  (3 << (0 * 2));      // Set analog mode

    // PUPDR0 = 00 (no pull-up/down)
    GPIOA->PUPDR &= ~(3 << (0 * 2));

    // === PA5: Output push-pull (LED) ===
    // MODER5 = 01 (general purpose output)
    GPIOA->MODER &= ~(3 << (5 * 2));      // Clear MODER5
    GPIOA->MODER |=  (1 << (5 * 2));      // Set output mode

    // OTYPER5 = 0 (push-pull)
    GPIOA->OTYPER &= ~(1 << 5);

    // OSPEEDR5 = 00 (low speed, or set to 01/10 if needed)
    GPIOA->OSPEEDR &= ~(3 << (5 * 2));    // Low speed (default)

    // PUPDR5 = 00 (no pull-up/down)
    GPIOA->PUPDR &= ~(3 << (5 * 2));
}
