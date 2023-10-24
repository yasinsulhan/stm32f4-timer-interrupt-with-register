#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

volatile unsigned int pressed;
volatile unsigned int waiting;

void RCC_Config()
{
	RCC->CR |= 0x00010000;	// HSEON and HSEONRDY enable
	while(!(RCC->CR & 0x00020000));	// HSEON Ready Flag wait
	RCC->CR |= 0x00080000;	// CSS Enable
	RCC->PLLCFGR |= 0x00400000;	// PLL e HSE seçtik
	RCC->PLLCFGR |= 0x00000004;	// PLL M = 4
	RCC->PLLCFGR |= 0x00005A00;	// Pll N = 168
	RCC->PLLCFGR |= 0x00000000;	// PLL p = 2
	RCC->CFGR |= 0x00000000;	// AHB Prescaler = 1
	RCC->CFGR |= 0x00080000;	// APB2 Prescaler = 2
	RCC->CFGR |= 0x00001400;	// APB1 Prescaler = 4
	RCC->CIR |= 0x00800000;		// CSS Flag clear
}

void GPIO_Config()
{
	RCC->AHB1ENR = 0x00000001; 		// GPIOA and GPIOD active

	GPIOD->MODER = 0x01000000;		// 12-15 output
	GPIOD->OTYPER = 0x00000000;		// push-pull
	GPIOD->OSPEEDR = 0xFF000000;	// High Speed
	GPIOD->PUPDR = 0x00000000;		// no pull-up, pull-down
}

void TIM_Config()
{
	RCC->APB1ENR |= 1 << 0;		// TIM2 clock enable
	TIM2->CR1 |= 0 << 4;		// Counter mode is upcounter (DIR)
	TIM2->CR1 |= 0 << 5;		// Center aligned mode edge-aligned mode (CMS)
	TIM2->CR1 |= 0 << 8;		// Set the clock divisixon to 1 (CKD)
	TIM2->SMCR |= 0 << 0;		// Internal clock source (SMS)
	TIM2->EGR |= 1 << 0;		// Týmer Update Generation ( UG: Update generation)
	TIM2->PSC |= 41999;			// 84000 MHz / (41999+1) = 2000 ( PSC[15:0]: Prescaler value) -> The counter clock frequency CK_CNT is equal to fCK_PSC / (PSC[15:0] + 1)
	TIM2->ARR |= 2000;			// 2000/2000 = 1 saniye => 1 saniyede bir 2000 (ARR=autoreload deðeri) saydirmis oluruz.

	TIM2->CR1 &= ~(1<<0);;		// Týmer2 Counter Disable for now (CEN:Counter enable)
}

void EXTI_Config()
{

	// Enable External Interrupt for Button
	SYSCFG->EXTICR[0] = 0x00000000;			// EXTI0 for PA0 button

	// Enable NVIC Interrupts
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(TIM2_IRQn);

	EXTI->IMR = 0x00000001; 	// PA0 bit is interrupt without masking
	EXTI->RTSR = 0x00000001;	// PA0 bit is rising edge
}

int main(void)
{

  pressed = 0;
  waiting = 0;

  while (1)
  {
	  if (pressed)
	  {
		  TIM2->EGR |= (1 << 0); // Reset timer
	      TIM2->CR1 |= (1 << 0); // Enable timer
	      waiting = 1;

	      while (waiting)
	      {
	    	  GPIOD->ODR |= (1 << 12);   // LED ON
	      }

	      GPIOD->ODR &= ~(1 << 12);  // LED OFF
	      pressed = 0;
	  }
}

void TIM2_IRQHandler(void)
{
  	waiting = 0;
  	TIM2->SR &= ~(1 << 0); // Clear UIF update interrupt flag
}

void EXTI0_IRQHandler(void)
{
  	pressed = 1;
  	EXTI->PR |= (1 << 0);  // Clear PR to re-enable EXTI interrupt
}



void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
