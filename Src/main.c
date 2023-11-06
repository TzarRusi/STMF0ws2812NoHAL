#include "stm32f0xx.h"

//количество светодиодов
#define leds 8

volatile uint32_t time = 0;
uint8_t pwm_values[24 * leds + 1];



void set_led_color(uint8_t led, uint8_t r, uint8_t g, uint8_t b)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
		pwm_values[24*led + i] = ((r >> (7 - i)) & 1)? 36: 13;
	for (i = 0; i < 8; i++)
		pwm_values[24*led + 8 + i] = ((g >> (7 - i)) & 1)? 36: 13;
	for (i=0; i < 8; i++)
		pwm_values[24*led + 16 + i] = ((b >> (7 - i)) & 1)? 36: 13;

	pwm_values[sizeof(pwm_values) / sizeof(pwm_values[0]) - 1] = 0;
}

void update_ws2812(void)
{
	// Disable DMA
	DMA1_Channel4->CCR &= ~DMA_CCR_EN;
	// Enable DMA clock
	RCC->AHBENR |= RCC_AHBENR_DMAEN;
	// DMA settings
	DMA1_Channel4->CCR = DMA_CCR_PL_1 | DMA_CCR_PL_0 | DMA_CCR_PSIZE_0 | DMA_CCR_MINC | DMA_CCR_DIR;
	//Target
	DMA1_Channel4->CPAR = (uint32_t)&TIM3->CCR1;
	//Source
	DMA1_Channel4->CMAR = (uint32_t)pwm_values;
	// Number of transfer
	DMA1_Channel4->CCR = DMA_CCR_EN;
}


//функция,включающая прерывания позволяющая делать простые задержки
void SysTick_Handler(void)
{
	time++;
}

int main(void)
{
	SystemInit();

	//x12 множитель для 48MHz
	RCC->CFGR &= ~(RCC_CFGR_PLLMUL_2 | RCC_CFGR_PLLMUL_0);
	RCC->CFGR |= RCC_CFGR_PLLMUL_3 | RCC_CFGR_PLLMUL_1;
	//Turn PLL on
	RCC->CR |= RCC_CR_PLLON;
	// Wait for it
	while(!(RCC->CR & RCC_CR_PLLRDY));

	// Select PLL as main clock
	RCC->CFGR &= ~RCC_CFGR_SW_0;
	RCC->CFGR |= RCC_CFGR_SW_1;

	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);

	//Tim3 clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	//Prescaler
	TIM3->PSC = 0;
	//Auto reload register - таймер считает до 100
	TIM3->ARR = 48;
	//На линии ос1реф, из которой сигнал идет на 1 канал записываем биты для включения шим на первом канале
	TIM3->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE;
	//Turn on 1 channel
	TIM3->CCER |= TIM_CCER_CC1E;
	// Enable DMA for CC1
	TIM3->DIER |= TIM_DIER_CC1DE;




	//НОГА МК НА РАБОТУ С ТАЙМЕРОМ GPIOA clock
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	// ПИН 6 В АЛЬТЕРНАТИВНОМ РЕЖИМЕ
	GPIOA->MODER |= GPIO_MODER_MODER6_1;
	//1 КАНАЛ 3 ТАЙМЕРА НАХОДИМ ПО ТАБЛИЦЕ TIM3 CH1 (AF1)
	GPIOA->AFR[0] |= 1 << (6 * 4);
	//PIN 6 - HIGH SPEED
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR10_1	|	GPIO_OSPEEDR_OSPEEDR10_0;


	//ЗАПУСКАЕМ ТАЙМЕР CEN = COUNTER ENABLE
	TIM3->CR1 |= TIM_CR1_CEN;



   while(1)
   {
	   set_led_color(0, 0xFF, 0xFF, 0xFF);
	   set_led_color(0, 0xFF, 0xFF, 0xFF);
	   set_led_color(2, 0, 0, 0xFF);
	   update_ws2812();
	   time = 0;
	   while (time < 500);

	   set_led_color(0, 0xFF, 0, 0);
	   set_led_color(1, 0, 0xFF, 0);
	   set_led_color(2, 0, 0, 0xFF);
	   update_ws2812();
	   time = 0;
	   while (time < 500);

	   set_led_color(1, 0xFF, 0, 0);
	   set_led_color(2, 0, 0xFF, 0);
	   set_led_color(0, 0, 0, 0xFF);
	   update_ws2812();
	   time = 0;
	   while (time < 500);


	   set_led_color(2, 0xFF, 0, 0);
	   set_led_color(0, 0, 0xFF, 0);
	   set_led_color(1, 0, 0, 0xFF);
	   update_ws2812();
	   time = 0;
	   while (time < 500);



   }
}
