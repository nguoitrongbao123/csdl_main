#include "main.h"
#include "ldr.h"
#include "i2c.h"
#include "tim.h"
#include "dht11.h"
#include "system_config.h"
#include "soil_analog.h"
#include "pump.h"
#include <stdio.h>
#include <string.h>

extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim1;
extern ADC_HandleTypeDef hadc1;

int main(void)
{
    HAL_Init();                 // Reset all peripherals and initialize HAL
SystemClock_Config();           // Configure system clock

MX_I2C1_Init();                 // Initialize I2C1 peripheral
MX_TIM1_Init();                 // Initialize Timer 1
MX_ADC1_Init();                 // Initialize ADC (for soil sensor)
MX_GPIO_Init();                 // Initialize GPIO ports

    HAL_TIM_Base_Start(&htim1);  // Start base timer (may be used for delay or PWM timing)
DHT11_Init();                    // Initialize DHT11 temperature & humidity sensor
Soil_Analog_Init();              // Initialize ADC settings for soil sensor
Pump_Init();                     // Setup pump output pin


    uint8_t temperature = 0, humidity = 0;
    uint16_t ldr_value;
    uint8_t soil_percent;
    uint8_t pumpStatus = 0;
    uint8_t lightStatus = 0;
    char msg[128];                // Buffer to send data to ESP32

    while (1)
    {
        // ==== Read DHT11 ====
        if (DHT11_Read_Data(&temperature, &humidity) != 0) {
            temperature = 0;
            humidity = 0;
        }

         // ==== Read Light (LDR) ====
        ldr_value = LDR_Read();
        if (ldr_value < 200) {
            GPIOA->BSRR = (1 << 5);         // Set PA5 HIGH to turn ON light
            lightStatus = 1;
        } else {
            GPIOA->BSRR = (1 << (5 + 16));  // Reset PA5 (set LOW) to turn OFF light
            lightStatus = 0;
        }


        // ==== Read Soil Moisture ====
        soil_percent = Soil_ReadPercent();
        if (soil_percent < 50) {             // Turn ON pump if soil is dry
            Pump_On();
            pumpStatus = 1;
        } else {                             // Turn OFF pump if soil is wet enough
            Pump_Off();
            pumpStatus = 0;
        }


        sprintf(msg, "TEMP:%d,HUM:%d,SOIL:%d,P:%d,D:%d\n",
                temperature, humidity, soil_percent, pumpStatus, lightStatus);

        HAL_I2C_Master_Transmit(&hi2c1, 0x42 << 1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

        HAL_Delay(2000);                      // Wait 2 seconds before next measurement
    }
}
