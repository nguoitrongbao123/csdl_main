#include "pump.h"

// Function to initialize the pump control pin
void Pump_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();                // Enable clock for GPIOA to allow configuration and usage

    GPIO_InitTypeDef GPIO_InitStruct = {0};      // Structure for GPIO configuration

    GPIO_InitStruct.Pin = PUMP_PIN;              // Select the pin connected to the pump control (relay/MOSFET)
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // Set as output in push-pull mode
    GPIO_InitStruct.Pull = GPIO_NOPULL;          // No internal pull-up or pull-down resistors
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // Low speed (saves power, sufficient for switching)

    HAL_GPIO_Init(PUMP_PORT, &GPIO_InitStruct);  // Initialize the GPIO with above settings
}

// Function to turn the pump ON
void Pump_On(void)
{
    HAL_GPIO_WritePin(PUMP_PORT, PUMP_PIN, GPIO_PIN_SET); // Set the pin HIGH → activate relay or MOSFET to turn on the pump
}

// Function to turn the pump OFF
void Pump_Off(void)
{
    HAL_GPIO_WritePin(PUMP_PORT, PUMP_PIN, GPIO_PIN_RESET); // Set the pin LOW → deactivate and turn off the pump
}
