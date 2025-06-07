#include "dht11.h"

#define DHT11_PORT GPIOA
#define DHT11_PIN GPIO_PIN_1

extern TIM_HandleTypeDef htim1;

void delay_us(uint16_t us)
{
    __HAL_TIM_SET_COUNTER(&htim1, 0);
    while (__HAL_TIM_GET_COUNTER(&htim1) < us);
}

void DHT11_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
}

int DHT11_Read_Data(uint8_t *temperature, uint8_t *humidity)
{
    uint8_t data[5] = {0};

    // === Step 1: Send start signal ===
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Đảm bảo đã cấp xung GPIOA

    // PA1 output mode
    GPIOA->MODER &= ~(3 << (1 * 2));
    GPIOA->MODER |=  (1 << (1 * 2));     // Output
    GPIOA->OTYPER &= ~(1 << 1);          // Push-pull
    GPIOA->OSPEEDR |= (2 << (1 * 2));    // High speed
    GPIOA->PUPDR &= ~(3 << (1 * 2));     // No pull

    GPIOA->BSRR = (1 << (1 + 16));       // Pull LOW
    HAL_Delay(20);                       // Keep low for >18ms

    GPIOA->BSRR = (1 << 1);              // Pull HIGH
    delay_us(30);                        // Wait 20–40us

    // === Step 2: Switch to input ===
    GPIOA->MODER &= ~(3 << (1 * 2));     // Input mode
    GPIOA->PUPDR &= ~(3 << (1 * 2));     // No pull

    // === Step 3: Check response from DHT11 ===
    if ((GPIOA->IDR & (1 << 1))) return 1;  // Không có phản hồi

    while (!(GPIOA->IDR & (1 << 1)));  // Đợi lên cao
    while ((GPIOA->IDR & (1 << 1)));   // Đợi xuống thấp

    // === Step 4: Đọc 40 bit dữ liệu ===
    for (int i = 0; i < 40; i++)
    {
        while (!(GPIOA->IDR & (1 << 1)));   // Đợi bit lên

        delay_us(40);                       // Delay khoảng 40us

        if ((GPIOA->IDR & (1 << 1)))        // Nếu vẫn còn HIGH → bit = 1
            data[i / 8] |= (1 << (7 - (i % 8)));

        while ((GPIOA->IDR & (1 << 1)));    // Đợi xuống LOW
    }

    // === Step 5: Kiểm tra checksum ===
    if ((uint8_t)(data[0] + data[1] + data[2] + data[3]) != data[4])
        return 2;

    *humidity = data[0];
    *temperature = data[2];
    return 0;
}
