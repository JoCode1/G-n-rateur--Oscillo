# Generateur-de-signaux

#include "stm32f4xx_hal_conf.h"
#include "stm32f429_Discovery_conf.h"

TIM_HandleTypeDef monTimer;
// TIM2 Handler global variable
int LED_Tim2Count = 0;

void LED_Init(){
	
	// enable ports
	int *RCC_AHB1ENR;
	RCC_AHB1ENR = (int*) 0x40023830;
	*RCC_AHB1ENR |= 1<<6;
	
	// configuration of G13
	int *GPIOG_MODER;
	GPIOG_MODER = (int*) 0x40021800;
	*GPIOG_MODER &= 0xF3FFFFFF;
	*GPIOG_MODER |= 0x04000000;
		
	int *GPIOG_OTYPER;
	GPIOG_OTYPER = (int*) 0x40021804;
	*GPIOG_OTYPER &= 0xFFFFDFFF;
	
	int *GPIOG_OSPEEDR;
	GPIOG_OSPEEDR = (int*) 0x40021808;
	*GPIOG_OSPEEDR &= 0xF3FFFFFF;
		
	int *GPIOG_PUPDR;
	GPIOG_PUPDR = (int*) 0x4002180C;
	*GPIOG_PUPDR &= 0xF3FFFFFF;
		
	// configuration of G14
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Pin = GPIO_PIN_14;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
	
	// enable Timer2
	int *RCC_APB1ENR;
	RCC_APB1ENR= (int*) 0x40023840;
	*RCC_APB1ENR |= 1 ;
	
	// config IT TIM2 : enable interrupt
	monTimer.Instance = TIM2; 
	monTimer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; //DIV2 ET DIV4
	monTimer.Init.CounterMode = TIM_COUNTERMODE_UP; 
	monTimer.Init.Period = 5000-1; //ARR value
	monTimer.Init.Prescaler = 4500-1; //PSC
	
	HAL_TIM_Base_Init(&monTimer);
	HAL_TIM_Base_Start_IT(&monTimer);
	
}

void LED_DispRed(int val){
	// RED : G14
	int *GPIOG_ODR;
	GPIOG_ODR = (int*) 0x40021814;
	
	if(val == 1) {
		*GPIOG_ODR |= 1<<14;
	}
	else {
		*GPIOG_ODR &= 0xBFFF;
	}	
}

void LED_DispGreen(int val){
	// GREEN : G13
	int *GPIOG_ODR;
	GPIOG_ODR = (int*) 0x40021814;
	
	if(val == 1) {
		*GPIOG_ODR |= 1<<13;
	}
	else {
		*GPIOG_ODR &= 0xDFFF;
	}
		
}


void TIM2_IRQHandler(void){
	
	__HAL_TIM_CLEAR_IT(&monTimer, TIM_IT_UPDATE); 
	LED_Tim2Count++;
	// #13 Make the LED blink!
	LED_DispRed(LED_Tim2Count%2);

}
