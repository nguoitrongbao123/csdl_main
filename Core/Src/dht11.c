#include "dht11.h"

// Define the port and pin where the DHT11 is connected
#define DHT11_PORT GPIOA
#define DHT11_PIN GPIO_PIN_1

// Timer handle used for microsecond delay
extern TIM_HandleTypeDef htim1;

// Function to provide microsecond delay using TIM1
void delay_us(uint16_t us)
{
    __HAL_TIM_SET_COUNTER(&htim1, 0);           // Reset the counter
    while (__HAL_TIM_GET_COUNTER(&htim1) < us); // Wait until the counter reaches the desired delay
}

// Initialize GPIO clock for DHT11
void DHT11_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE(); // Enable clock for GPIOA
}

// Function to read temperature and humidity from DHT11
// Returns: 0 = OK, 1 = No response, 2 = Checksum error
int DHT11_Read_Data(uint8_t *temperature, uint8_t *humidity)
{
    uint8_t data[5] = {0}; // Array to store 5 bytes of data from sensor

    // === Step 1: Send start signal to DHT11 ===
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Ensure GPIOA clock is enabled

    // Configure PA1 as output
    GPIOA->MODER &= ~(3 << (1 * 2));
    GPIOA->MODER |=  (1 << (1 * 2));     // Set as output mode
    GPIOA->OTYPER &= ~(1 << 1);          // Push-pull output
    GPIOA->OSPEEDR |= (2 << (1 * 2));    // High speed
    GPIOA->PUPDR &= ~(3 << (1 * 2));     // No pull-up/pull-down

    GPIOA->BSRR = (1 << (1 + 16));       // Drive PA1 LOW
    HAL_Delay(20);                       // Wait at least 18ms

    GPIOA->BSRR = (1 << 1);              // Drive PA1 HIGH
    delay_us(30);                        // Wait for 20–40 microseconds

    // === Step 2: Switch PA1 to input mode ===
    GPIOA->MODER &= ~(3 << (1 * 2));     // Input mode
    GPIOA->PUPDR &= ~(3 << (1 * 2));     // No pull-up/pull-down

    // === Step 3: Wait for DHT11 response ===
    if ((GPIOA->IDR & (1 << 1))) return 1;  // If pin is HIGH, sensor didn’t respond

    while (!(GPIOA->IDR & (1 << 1)));  // Wait for LOW to HIGH transition
    while ((GPIOA->IDR & (1 << 1)));   // Wait for HIGH to LOW transition

    // === Step 4: Read 40 bits of data (5 bytes) ===
    for (int i = 0; i < 40; i++)
    {
        while (!(GPIOA->IDR & (1 << 1)));   // Wait for HIGH signal (start of bit)

        delay_us(40);                       // Wait ~40us

        // If still HIGH after 40us → it's a 1, otherwise it's a 0
        if ((GPIOA->IDR & (1 << 1)))
            data[i / 8] |= (1 << (7 - (i % 8))); // Store the bit

        while ((GPIOA->IDR & (1 << 1)));    // Wait until line goes LOW
    }

    // === Step 5: Verify checksum ===
    if ((uint8_t)(data[0] + data[1] + data[2] + data[3]) != data[4])
        return 2; // Checksum mismatch

    // Assign output values
    *humidity = data[0];
    *temperature = data[2];
    return 0; // Success
}
