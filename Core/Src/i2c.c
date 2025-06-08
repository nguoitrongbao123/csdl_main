#include "i2c.h"

     // Khai báo một biến điều khiển I2C
I2C_HandleTypeDef hi2c1;
     // Hàm khởi tạo I2C1
void MX_I2C1_Init(void)
{
       // === 1. Bật clock cho GPIOB và I2C1 ===
     __HAL_RCC_GPIOB_CLK_ENABLE();     // Bật clock cho port GPIOB (nơi chân SDA/SCL kết nối)
    __HAL_RCC_I2C1_CLK_ENABLE();      // Bật clock cho peripheral I2C1

        // === 2. Cấu hình chân SDA (PB7) và SCL (PB6) ===
    GPIO_InitTypeDef GPIO_InitStruct = {0};           // Khởi tạo cấu trúc cấu hình GPIO
    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7; // Chọn chân PB6 (SCL) và PB7 (SDA)
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;       // Chế độ Alternate Function, Open-Drain (theo chuẩn I2C)
    GPIO_InitStruct.Pull = GPIO_PULLUP;           // Kích hoạt điện trở kéo lên (pull-up)
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // Tốc độ rất cao
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;    // Gán chức năng chân là AF4 (tương ứng I2C1)
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);       // Áp dụng cấu hình cho GPIOB


   // === 3. Cấu hình các tham số cho I2C1 ===
    hi2c1.Instance = I2C1;                  // Chọn peripheral là I2C1
    hi2c1.Init.ClockSpeed = 100000;         // Tốc độ I2C 100kHz (chuẩn)
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2; // Tỉ lệ chu kỳ (duty cycle) là 2
    hi2c1.Init.OwnAddress1 = 0;             // Địa chỉ của thiết bị (0 nếu là Master)
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT; // Dùng định dạng địa chỉ 7-bit
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE; // Không dùng địa chỉ thứ 2
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE; // Không cho phép general call
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;     // Cho phép chế độ "clock stretching"
    
    HAL_I2C_Init(&hi2c1); // Gọi hàm HAL để khởi tạo I2C với các thông số trên
}
