#include "pump.h"

void Pump_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();                // Bật clock cho GPIOA để có thể cấu hình và sử dụng

    GPIO_InitTypeDef GPIO_InitStruct = {0};      // Cấu trúc cấu hình GPIO

    GPIO_InitStruct.Pin = PUMP_PIN;              // Chọn chân điều khiển máy bơm
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // Chế độ output, push-pull
    GPIO_InitStruct.Pull = GPIO_NOPULL;          // Không dùng điện trở kéo lên/kéo xuống
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // Tốc độ thấp (tiết kiệm năng lượng, đủ dùng)

    HAL_GPIO_Init(PUMP_PORT, &GPIO_InitStruct);  // Áp dụng cấu hình cho port
}

void Pump_On(void)
{
    HAL_GPIO_WritePin(PUMP_PORT, PUMP_PIN, GPIO_PIN_SET);   // Xuất mức HIGH → kích rơ-le hoặc mosfet bật bơm
}

void Pump_Off(void)
{
    HAL_GPIO_WritePin(PUMP_PORT, PUMP_PIN, GPIO_PIN_RESET);  // Xuất mức LOW → tắt bơm
}
