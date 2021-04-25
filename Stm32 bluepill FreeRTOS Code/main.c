#include "stm32f10x.h"                  // Device header
#include <FreeRTOS.h>
#include <task.h>

/*
Boiler plate FreeRTOS code
*/

void Clocks_Init(void);
void GPIOC_Init(void);
void vTask1(void *pvParameters);

int main(void)
{
	//MCU configurations
	Clocks_Init();
	GPIOC_Init();
	
	xTaskCreate(vTask1, "Main Task", 256 , NULL, 1, NULL);
	// Startup of the FreeRTOS scheduler.  The program should block here.  
	vTaskStartScheduler();
	for(;;);
	
}

void Clocks_Init(void)
{
	while( (RCC->CR & RCC_CR_HSIRDY) != RCC_CR_HSIRDY ); //wait for internal oscillator to be stable
	RCC->CFGR &= ~RCC_CFGR_SW; //use internal oscillator as system clock
	RCC->CFGR &= ~RCC_CFGR_PPRE1; //No APB1 prescaler, run it at 8MHz
	RCC->CR &= ~RCC_CR_PLLON;	//disable PLL
	RCC->CR &= ~RCC_CR_HSEON; //disable external oscillator
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; //enable clock for I/O port C
}

void GPIOC_Init(void)
{
	GPIOC->CRH |= GPIO_CRH_MODE13_1; //port C output mode (maximum speed = 2MHz)
	GPIOC->CRH &= ~GPIO_CRH_CNF13; //port C output push pull mode
}

void vTask1(void *pvParameters)
{
	for(;;)
	{
		GPIOC->ODR |= GPIO_ODR_ODR13;
		vTaskDelay(pdMS_TO_TICKS(2000));
		GPIOC->ODR &= ~GPIO_ODR_ODR13;
		vTaskDelay(pdMS_TO_TICKS(2000));
	}
	
}

