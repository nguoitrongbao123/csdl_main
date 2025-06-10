#include "ldr.h"

// Declare handler for ADC1
ADC_HandleTypeDef hadc1;

// Function to initialize ADC1
void MX_ADC1_Init(void)
{
    __HAL_RCC_ADC1_CLK_ENABLE();                    // Enable clock for ADC1

    hadc1.Instance = ADC1;                          // Use ADC1 peripheral
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;     // 12-bit resolution (range 0–4095)
    hadc1.Init.ScanConvMode = DISABLE;              // Disable scan mode (only one channel)
    hadc1.Init.ContinuousConvMode = DISABLE;        // Disable continuous conversion
    hadc1.Init.DiscontinuousConvMode = DISABLE;     // Disable discontinuous conversion
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;     // Right-align ADC data
    hadc1.Init.NbrOfConversion = 1;                 // One conversion only
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START; // Start conversion by software trigger

    HAL_ADC_Init(&hadc1);                           // Initialize ADC1 with the above settings
}

// Function to read value from LDR (Light Dependent Resistor)
uint16_t LDR_Read(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_0;              // Use ADC channel 0 (PA0)
    sConfig.Rank = 1;                              // First and only conversion
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES; // Fast sampling time
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);       // Apply the configuration

    HAL_ADC_Start(&hadc1);                         // Start ADC conversion
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); // Wait for conversion to finish
    uint16_t value = HAL_ADC_GetValue(&hadc1);     // Get the converted value
    HAL_ADC_Stop(&hadc1);                          // Stop ADC after reading

    return value; // Return light level (0 = dark, 4095 = very bright)
}

// GPIO initialization function
void MX_GPIO_Init(void)
{
    // 1. Enable clock for GPIOA
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // === PA0: Configure as analog input for LDR ===
    GPIOA->MODER &= ~(3 << (0 * 2)); // Clear mode bits for PA0
    GPIOA->MODER |=  (3 << (0 * 2)); // Set to analog mode (MODER0 = 11)

    GPIOA->PUPDR &= ~(3 << (0 * 2)); // Disable pull-up/pull-down resistors (PUPDR0 = 00)

    // === PA5: Configure as output (for LED, for example) ===
    GPIOA->MODER &= ~(3 << (5 * 2)); // Clear mode bits for PA5
    GPIOA->MODER |=  (1 << (5 * 2)); // Set to output mode (MODER5 = 01)

    GPIOA->OTYPER &= ~(1 << 5);      // Push-pull output (OTYPER5 = 0)

    GPIOA->OSPEEDR &= ~(3 << (5 * 2)); // Set low speed (can be increased if needed)

    GPIOA->PUPDR &= ~(3 << (5 * 2)); // No pull-up or pull-down on PA5
}
