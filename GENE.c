
#include "stm32f4xx_hal_conf.h"
#include "stm32f429_Discovery_conf.h"

TIM_HandleTypeDef TimerPE3;

void GENE_Init(){
	// PE3
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	GPIOE->MODER&= 0xFFFFFF7F;
	GPIOE->MODER|=1<<6;
	GPIOE->OTYPER&=0xFFFFFFF7;
	GPIOE->OSPEEDR&=0xFFFFFF3F;
	GPIOE->PUPDR&=0xFFFFFF3F;
	
	RCC->APB1ENR |= (1<<2); //enable Timer4
	TimerPE3.Instance = TIM4;
	TimerPE3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;//DIV2 et DIV4
	TimerPE3.Init.CounterMode = TIM_COUNTERMODE_UP;
	TimerPE3.Init.Period = 10-1; //ARR value
	TimerPE3.Init.Prescaler = 4500-1; //PSC 
	HAL_TIM_Base_Init(&TimerPE3); 
	HAL_TIM_Base_Start_IT(&TimerPE3);

	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	HAL_NVIC_SetPriority(TIM4_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(TIM4_IRQn); 
	
	//#30 Enable for Button
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	SYSCFG->EXTICR[0]&=0xFFFFFFF0;
	EXTI->RTSR&=0xFFFFFFFE;
	EXTI->RTSR|=1;
	EXTI->FTSR&=0xFFFFFFFE;
	EXTI->IMR&=0xFFFFFFFE;
	EXTI->IMR|=1;
	
	HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
  	NVIC_EnableIRQ(EXTI0_IRQn);
	
}

void GENE_TogglePE3(void){
	if((GPIOE->ODR&0x000000008)==0x00000000){
		GPIOE->ODR&=0xFFFFFFF7;
		GPIOE->ODR|=1<<3;
	} else {
		GPIOE->ODR&=0xFFFFFFFF7;
	}
}

void TIM4_IRQHandler(void) {
	__HAL_TIM_CLEAR_IT(&TimerPE3, TIM_IT_UPDATE); 
	GENE_TogglePE3();
}

void GENE_SetFreqPE3(int f){
	int fAPB1 = HAL_RCC_GetPCLK1Freq();
	TIM4->ARR = (fAPB1/((TIM4->PSC +1)*f))-1;
}

void EXTI0_IRQHandler(void) {
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
} 
