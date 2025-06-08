#include "soil_analog.h"

extern ADC_HandleTypeDef hadc1;
void Soil_Analog_Init(void)
{
    // 1. Enable GPIOA clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // 2. Set PA4 to analog mode (MODER4 = 11)
    GPIOA->MODER &= ~(3 << (4 * 2));   // Clear mode bits
    GPIOA->MODER |=  (3 << (4 * 2));   // Set analog mode (11)

    // 3. No pull-up, no pull-down (PUPDR4 = 00)
    GPIOA->PUPDR &= ~(3 << (4 * 2));   // No pull
}


uint16_t Soil_ReadRaw(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = SOIL_ADC_CHANNEL;            // Kênh ADC tương ứng với PA4
    sConfig.Rank = 1;                              // Ưu tiên đọc đầu tiên
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES; // Thời gian lấy mẫu nhanh

    HAL_ADC_ConfigChannel(&hadc1, &sConfig);       // Cấu hình kênh ADC

    HAL_ADC_Start(&hadc1);                         // Bắt đầu chuyển đổi
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); // Chờ đến khi xong
    uint16_t val = HAL_ADC_GetValue(&hadc1);       // Đọc giá trị
    HAL_ADC_Stop(&hadc1);                          // Dừng ADC

    return val;
}

uint8_t Soil_ReadPercent(void)
{
    uint16_t raw = Soil_ReadRaw(); // Đọc ADC thô

    const uint16_t WET_RAW = 2700; // Ngưỡng đất ẩm nhất (ướt)
    const uint16_t DRY_RAW = 3600; // Ngưỡng đất khô nhất

    if (raw <= WET_RAW) return 100;  // Nếu nhỏ hơn mức ướt → 100%
    if (raw >= DRY_RAW) return 0;    // Nếu lớn hơn mức khô → 0%

    return (DRY_RAW - raw) * 100 / (DRY_RAW - WET_RAW);
}
