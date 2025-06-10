#include "i2c.h"

// Declare an I2C handle for I2C1
I2C_HandleTypeDef hi2c1;

// Function to initialize I2C1
void MX_I2C1_Init(void)
{
    // === 1. Enable clocks for GPIOB and I2C1 peripheral ===
    __HAL_RCC_GPIOB_CLK_ENABLE();     // Enable clock for GPIOB (used for SDA and SCL)
    __HAL_RCC_I2C1_CLK_ENABLE();      // Enable clock for the I2C1 peripheral

    // === 2. Configure SDA (PB7) and SCL (PB6) pins ===
    GPIO_InitTypeDef GPIO_InitStruct = {0};              // Initialize GPIO config structure
    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;       // Select PB6 (SCL) and PB7 (SDA)
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;              // Set mode to Alternate Function Open-Drain (standard for I2C)
    GPIO_InitStruct.Pull = GPIO_PULLUP;                  // Enable internal pull-up resistors
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;   // Set very high speed
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;           // Assign Alternate Function 4 (AF4) for I2C1
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);              // Initialize GPIOB with this configuration

    // === 3. Configure I2C1 peripheral parameters ===
    hi2c1.Instance = I2C1;                              // Use I2C1 peripheral
    hi2c1.Init.ClockSpeed = 100000;                     // Set I2C speed to 100kHz (standard mode)
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;             // Use 1:1 duty cycle (standard)
    hi2c1.Init.OwnAddress1 = 0;                         // Device address (0 if master)
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT; // Use 7-bit addressing
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE; // Disable dual addressing mode
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE; // Disable general call mode
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;     // Enable clock stretching

    // Initialize the I2C peripheral with the configuration
    HAL_I2C_Init(&hi2c1);
}
