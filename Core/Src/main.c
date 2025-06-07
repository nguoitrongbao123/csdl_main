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
    HAL_Init();
    SystemClock_Config();
    MX_I2C1_Init();
    MX_TIM1_Init();
    MX_ADC1_Init();
    MX_GPIO_Init();

    HAL_TIM_Base_Start(&htim1);
    DHT11_Init();
    Soil_Analog_Init();
    Pump_Init();

    uint8_t temperature = 0, humidity = 0;
    uint16_t ldr_value;
    uint8_t soil_percent;
    uint8_t pumpStatus = 0;
    uint8_t lightStatus = 0;
    char msg[128];

    while (1)
    {
        // DHT11
        if (DHT11_Read_Data(&temperature, &humidity) != 0) {
            temperature = 0;
            humidity = 0;
        }

        // light
        ldr_value = LDR_Read();
        if (ldr_value < 200) {
            GPIOA->BSRR = (1 << 5);         // PA5 HIGH
            lightStatus = 1;
        } else {
            GPIOA->BSRR = (1 << (5 + 16));  // PA5 LOW
            lightStatus = 0;
        }


        // soil hum
        soil_percent = Soil_ReadPercent();
        if (soil_percent < 50) {
            Pump_On();
            pumpStatus = 1;
        } else {
            Pump_Off();
            pumpStatus = 0;
        }


        sprintf(msg, "TEMP:%d,HUM:%d,SOIL:%d,P:%d,D:%d\n",
                temperature, humidity, soil_percent, pumpStatus, lightStatus);

        HAL_I2C_Master_Transmit(&hi2c1, 0x42 << 1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

        HAL_Delay(2000);
    }
    // ==== ĐỌC LỆNH TỪ ESP32 ====
char controlBuffer[32] = {0};
HAL_I2C_Master_Receive(&hi2c1, 0x42 << 1, (uint8_t*)controlBuffer, sizeof(controlBuffer), HAL_MAX_DELAY);

// ==== PHÂN TÍCH CHUỖI ====
if (strstr(controlBuffer, "P:1")) {
    Pump_On();
    pumpStatus = 1;
} else if (strstr(controlBuffer, "P:0")) {
    Pump_Off();
    pumpStatus = 0;
}

if (strstr(controlBuffer, "D:1")) {
    GPIOA->BSRR = (1 << 5);         // PA5 HIGH
    lightStatus = 1;
} else if (strstr(controlBuffer, "D:0")) {
    GPIOA->BSRR = (1 << (5 + 16));  // PA5 LOW
    lightStatus = 0;
}

}
