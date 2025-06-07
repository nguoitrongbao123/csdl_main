#include "soil_analog.h"

extern ADC_HandleTypeDef hadc1;
void Soil_Analog_Init(void)
{
    // 1. Enable GPIOA clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // 2. Set PA4 to analog mode (MODER4 = 11)
    GPIOA->MODER &= ~(3 << (4 * 2));   // Clear mode bits
    GPIOA->MODER |=  (3 << (4 * 2));   // Set analog mode (11)

    // 3. No pull-up, no pull-down (PUPDR4 = 00)
    GPIOA->PUPDR &= ~(3 << (4 * 2));   // No pull
}


uint16_t Soil_ReadRaw(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = SOIL_ADC_CHANNEL;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    uint16_t val = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);

    return val;
}

uint8_t Soil_ReadPercent(void)
{
    uint16_t raw = Soil_ReadRaw();

    const uint16_t WET_RAW = 2700;
    const uint16_t DRY_RAW = 3600;

    if (raw <= WET_RAW) return 100;
    if (raw >= DRY_RAW) return 0;

    return (DRY_RAW - raw) * 100 / (DRY_RAW - WET_RAW);
}

