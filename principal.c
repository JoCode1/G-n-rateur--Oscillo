#include "stm32f4xx_hal_conf.h"
#include "stm32f429_Discovery_conf.h"

extern unsigned short int SCREEN_Font11x18 [];   
extern SCREEN_FontDef_t SCREEN_FormatFont_11x18; 

// Call of "IncTick" Handler
void SysTick_Handler(){
	HAL_IncTick();
}


void PRINCIPAL_Init(){
	
	SCREEN_Init();
	LED_Init();
	GENE_Init();
	SCREEN_Rotate(SCREEN_Orientation_Portrait_2); 
	SCREEN_Fill(SCREEN_COLOR_WHITE); 
	SCREEN_Puts(20, 130, "BONJOUR!", &SCREEN_FormatFont_11x18, SCREEN_COLOR_BLACK, SCREEN_COLOR_RED);
	
	// display of f1 and f2 using printf
	volatile int f1,f2; 
	f1 = HAL_RCC_GetSysClockFreq();  // SYSCLK
	f2 = HAL_RCC_GetPCLK1Freq(); // APB1
	set_cursor(0,0);
	printf("f1 : %d",f1);
	set_cursor(0,20);
	printf("f2 : %d",f2);

}

/*void SysTick_Handler()
{
	HAL_IncTick();
}*/

void test01(){

	PRINCIPAL_Init();
	volatile int SYSCLK = HAL_RCC_GetSysClockFreq();
	volatile int AHB = HAL_RCC_GetHCLKFreq();
	volatile int APB1 = HAL_RCC_GetPCLK1Freq();
	volatile int APB2 = HAL_RCC_GetPCLK2Freq();
	
	// Tests LED_Dispxxx
	LED_DispGreen(1);
	LED_DispGreen(0);
	LED_DispRed(1);
	LED_DispRed(0);
	while(1){
	}
}

int main(){
	test01();
	return 0;
}
