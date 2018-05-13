
#include "stm32f4xx_hal_conf.h"
#include "stm32f429_Discovery_conf.h"

TIM_HandleTypeDef TimerAcqu;
ADC_HandleTypeDef hadc;
int SPY_x=0;

// to clear window 1 2 or 3
void SPY_ClearWindow(int n){
	int x0,y0,x1,y1;
	if(n>0 && n<4){
		x0=0;
		y0=160+(n-1)*53;
		x1=240;
		y1=160+(n)*53-1;
		SCREEN_DrawFilledRectangle(x0,y0,x1,y1, SCREEN_COLOR_WHITE);
	}
}

// to draw a pixel in a window (1, 2 or 3) in portrait
// x is the horizontal position of the pixel (0 to 240)
// y is the value of the vertical position in the window
// from 0 to ech (maximum value of y in the window)
void SPY_DrawPixel(int x, int y, int ech, int n){
	int i,j,col;
	// i--> 
	// j|
	switch(n){
		case 1:
			col=SCREEN_COLOR_BLACK;
			break;
		case 2:
			col=SCREEN_COLOR_RED;
			break;
		case 3:
			col=SCREEN_COLOR_BLACK;
			break;
		default:
			return;
	}
	if(x<0 || x>240)
		return;
	i=x;
	if(y<0 || y>ech)
		return;
	j=160+n*53-3-((y*50)/ech);
	SCREEN_DrawPixel(i,j,col);		
}

void SPY_Init(){
	//GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC->AHB1ENR |= (1<<0); //enable GPIOA
	
	GPIOA->MODER &= 0xFFFFFF3F;
	GPIOA->MODER |= 3<<6;
	GPIOA->OTYPER &= 0xFFFFFFF7;
	GPIOA->OSPEEDR &= 0xFFFFFF3F;
	GPIOA->PUPDR &= 0xFFFFFF3F;
	
	RCC->APB2ENR |= (1<<8); //enable ADC1
	
	hadc.Instance = ADC1;
	hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	hadc.Init.DataAlign = ADC_DATAALIGN_LEFT;
	hadc.Init.Resolution = ADC_RESOLUTION_12B;
	hadc.Init.ScanConvMode = DISABLE;
	HAL_ADC_Init(&hadc);
	
	ADC1->SQR3 |= 3; //canal 3 en 1ère position
	ADC1->SMPR2 |= (3<<10); //temps conversion : 144 cycles
	
	HAL_NVIC_SetPriority(ADC_IRQn,2,0);
	HAL_NVIC_EnableIRQ(ADC_IRQn);
	
	RCC->APB1ENR |= (1<<3); //enable Timer5
	
	TimerAcqu.Instance = TIM5;
	TimerAcqu.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TimerAcqu.Init.CounterMode = TIM_COUNTERMODE_UP;
	TimerAcqu.Init.Period = 44999;
	TimerAcqu.Init.Prescaler = 9;
	HAL_TIM_Base_Init(&TimerAcqu);
	HAL_TIM_Base_Start_IT(&TimerAcqu);

	HAL_NVIC_SetPriority(TIM5_IRQn,2,0);
	HAL_NVIC_EnableIRQ(TIM5_IRQn);
		
}

void TIM5_IRQHandler(void){
	TIM5->SR &=~(1); //effacement de la demande
	HAL_ADC_Start_IT(&hadc);  //lancement de la conversion
}

void ADC_IRQHandler(void)
{
	int valeurConvertie;
	valeurConvertie = HAL_ADC_GetValue(&hadc); 
	SPY_DrawPixel(SPY_x,valeurConvertie,66000,1); 
	SPY_x = (SPY_x +1) % 240;
	if (SPY_x == 0){ 
		SPY_ClearWindow(1);
	} 
}

