#include "ldr.h"
    // Khai báo handler cho ADC1
ADC_HandleTypeDef hadc1;
    // Hàm khởi tạo ADC1
void MX_ADC1_Init(void)
{
    __HAL_RCC_ADC1_CLK_ENABLE();                    // Bật clock cho ADC1

    hadc1.Instance = ADC1;                            // Chọn ADC1
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;       // Độ phân giải 12 bit (0–4095)
    hadc1.Init.ScanConvMode = DISABLE;                // Không quét nhiều kênh
    hadc1.Init.ContinuousConvMode = DISABLE;          // Không chuyển đổi liên tục (chỉ khi gọi thủ công)
    hadc1.Init.DiscontinuousConvMode = DISABLE;       // Không chia nhỏ chuyển đổi
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;       // Canh phải dữ liệu ADC
    hadc1.Init.NbrOfConversion = 1;                   // Chỉ đọc 1 kênh
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START; // Bắt đầu chuyển đổi bằng phần mềm


    HAL_ADC_Init(&hadc1);                             // Gọi hàm khởi tạo ADC của HAL
}

uint16_t LDR_Read(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_0; // Đọc kênh ADC số 0 (tương ứng chân PA0)
    sConfig.Rank = 1;                // Ưu tiên chuyển đổi (duy nhất nên là 1)
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES; // Thời gian lấy mẫu (rất nhanh)
    HAL_ADC_ConfigChannel(&hadc1, &sConfig); // Áp dụng cấu hình cho kênh

    HAL_ADC_Start(&hadc1);                            // Bắt đầu chuyển đổi ADC
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); // Đợi kết quả
    uint16_t value = HAL_ADC_GetValue(&hadc1);        // Lấy giá trị đã chuyển đổi
    HAL_ADC_Stop(&hadc1);                             // Dừng ADC

    return value;                                              // Trả về giá trị ánh sáng (0 → tối, 4095 → sáng)
}
} 
void MX_GPIO_Init(void)
{
    // 1. Enable GPIOA clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // === PA0: Analog input (LDR) ===
    // MODER0 = 11 (analog mode)
    GPIOA->MODER &= ~(3 << (0 * 2));      // Clear MODER0
    GPIOA->MODER |=  (3 << (0 * 2));      // Set analog mode

    // PUPDR0 = 00 (no pull-up/down)
    GPIOA->PUPDR &= ~(3 << (0 * 2));

    // === PA5: Output push-pull (LED) ===
    // MODER5 = 01 (general purpose output)
    GPIOA->MODER &= ~(3 << (5 * 2));      // Clear MODER5
    GPIOA->MODER |=  (1 << (5 * 2));      // Set output mode

    // OTYPER5 = 0 (push-pull)
    GPIOA->OTYPER &= ~(1 << 5);

    // OSPEEDR5 = 00 (low speed, or set to 01/10 if needed)
    GPIOA->OSPEEDR &= ~(3 << (5 * 2));    // Low speed (default)

    // PUPDR5 = 00 (no pull-up/down)
    GPIOA->PUPDR &= ~(3 << (5 * 2));
}
