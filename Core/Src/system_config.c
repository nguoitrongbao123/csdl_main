#include "system_config.h"

void SystemClock_Config(void)
{
    // 1. Enable power interface clock
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    // 2. Set voltage scaling to Scale 1 (high performance)
    PWR->CR &= ~PWR_CR_VOS;
    PWR->CR |= PWR_CR_VOS_1;

    // 3. Enable HSI (High-Speed Internal 16 MHz oscillator)
    RCC->CR |= RCC_CR_HSION;

    // 4. Wait until HSI is ready
    while (!(RCC->CR & RCC_CR_HSIRDY));

    // 5. Select HSI as system clock (SYSCLK)
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_HSI;

    // 6. Wait until HSI is used as system clock
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);

    // 7. Set AHB, APB1, APB2 prescalers to /1
    RCC->CFGR &= ~RCC_CFGR_HPRE;   // AHB = SYSCLK / 1
    RCC->CFGR &= ~RCC_CFGR_PPRE1;  // APB1 = HCLK / 1
    RCC->CFGR &= ~RCC_CFGR_PPRE2;  // APB2 = HCLK / 1

    // 8. Optional: Set flash latency = 0 wait states (valid for 0–24 MHz)
    FLASH->ACR &= ~FLASH_ACR_LATENCY;
    FLASH->ACR |= FLASH_ACR_LATENCY_0WS;
}

#include "tim.h"

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
}
