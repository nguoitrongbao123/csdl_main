#include "soil_analog.h"

extern ADC_HandleTypeDef hadc1;

// Function to initialize the GPIO for analog soil sensor input
void Soil_Analog_Init(void)
{
    // 1. Enable GPIOA clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // 2. Set PA4 to analog mode (MODER4 = 11)
    GPIOA->MODER &= ~(3 << (4 * 2));   // Clear mode bits for PA4
    GPIOA->MODER |=  (3 << (4 * 2));   // Set to analog mode

    // 3. No pull-up or pull-down resistors (PUPDR4 = 00)
    GPIOA->PUPDR &= ~(3 << (4 * 2));   // Disable pull-up/down resistors
}

// Function to read raw ADC value from the soil sensor
uint16_t Soil_ReadRaw(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    sConfig.Channel = SOIL_ADC_CHANNEL;            // ADC channel assigned to PA4
    sConfig.Rank = 1;                              // Rank 1: first in conversion sequence
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES; // Short sampling time (fast)

    HAL_ADC_ConfigChannel(&hadc1, &sConfig);       // Configure the ADC channel

    HAL_ADC_Start(&hadc1);                         // Start ADC conversion
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); // Wait until conversion is complete
    uint16_t val = HAL_ADC_GetValue(&hadc1);       // Read the converted value
    HAL_ADC_Stop(&hadc1);                          // Stop the ADC

    return val;                                    // Return raw ADC value
}

// Function to convert raw ADC value into moisture percentage (0–100%)
uint8_t Soil_ReadPercent(void)
{
    uint16_t raw = Soil_ReadRaw(); // Read raw value from ADC

    const uint16_t WET_RAW = 2700; // ADC value when soil is very wet
    const uint16_t DRY_RAW = 3600; // ADC value when soil is very dry

    if (raw <= WET_RAW) return 100;  // If very wet, return 100%
    if (raw >= DRY_RAW) return 0;    // If very dry, return 0%

    // Linear interpolation between WET_RAW and DRY_RAW
    return (DRY_RAW - raw) * 100 / (DRY_RAW - WET_RAW);
}
