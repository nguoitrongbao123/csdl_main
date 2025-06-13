#include "dht11.h"

#define DHT11_PORT GPIOA
#define DHT11_PIN GPIO_PIN_1

TIM_HandleTypeDef htim1;

void MX_TIM1_Init(void)
{
    __HAL_RCC_TIM1_CLK_ENABLE();

    htim1.Instance = TIM1;
    htim1.Init.Prescaler = (SystemCoreClock / 1000000) - 1;
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1.Init.Period = 0xFFFF;
    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&htim1);
    HAL_TIM_Base_Start(&htim1);
}

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

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER &= ~(3 << (1 * 2));
    GPIOA->MODER |=  (1 << (1 * 2));
    GPIOA->OTYPER &= ~(1 << 1);
    GPIOA->OSPEEDR |= (2 << (1 * 2));
    GPIOA->PUPDR &= ~(3 << (1 * 2));

    GPIOA->BSRR = (1 << (1 + 16));
    HAL_Delay(20);

    GPIOA->BSRR = (1 << 1);
    delay_us(30);

    GPIOA->MODER &= ~(3 << (1 * 2));
    GPIOA->PUPDR &= ~(3 << (1 * 2));

    if ((GPIOA->IDR & (1 << 1))) return 1;

    while (!(GPIOA->IDR & (1 << 1)));
    while ((GPIOA->IDR & (1 << 1)));

    for (int i = 0; i < 40; i++)
    {
        while (!(GPIOA->IDR & (1 << 1)));
        delay_us(40);
        if ((GPIOA->IDR & (1 << 1)))
            data[i / 8] |= (1 << (7 - (i % 8)));
        while ((GPIOA->IDR & (1 << 1)));
    }

    if ((uint8_t)(data[0] + data[1] + data[2] + data[3]) != data[4])
        return 2;

    *humidity = data[0];
    *temperature = data[2];
    return 0;
}

