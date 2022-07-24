#include <LPC17xx.h>
#include <cmsis_os.h>
#include "osObjects.h"


void TIMER0_IRQHandler(void){
	if(LPC_TIM0->IR & 2) 	  						LPC_GPIO0->FIOSET|=1;
	else if 	(LPC_TIM0->IR & 1) 				   LPC_GPIO0->FIOCLR|=1;
	LPC_TIM0->IR=3;
}


void TIMER1_IRQHandler(void){
	if(LPC_TIM1->IR & 2) 	  						LPC_GPIO0->FIOSET|=2;
	else if 	(LPC_TIM1->IR & 1) 				   LPC_GPIO0->FIOCLR|=2;
	LPC_TIM1->IR=3;
}


void servo_A_lock(int angle){
	LPC_GPIO0->FIODIR|=1;
	LPC_GPIO0->FIOCLR|=1;
	LPC_TIM0->TCR=0;
	LPC_TIM0->PR=25-1;
	LPC_TIM0->MR0=20000; //20ms
	if (angle>90) angle=90;
	if (angle<-90) angle=-90;
	LPC_TIM0->MR1=1500+((angle*2000)/180); // -90-->500us and 90-->2500us
	LPC_TIM0->MCR=3|8;
	NVIC_EnableIRQ(TIMER0_IRQn);
	LPC_TIM0->TCR=1;
}


void servo_B_lock(int angle){
	LPC_GPIO0->FIODIR|=2;
	LPC_GPIO0->FIOCLR=1;
	LPC_TIM1->TCR=0;
	LPC_TIM1->PR=25-1;
	LPC_TIM1->MR0=20000; //20ms
	if (angle>90) angle=90;
	if (angle<-90) angle=-90;
	LPC_TIM1->MR1=1500+((angle*2000)/180); // -90-->500us and 90-->2500us
	LPC_TIM1->MCR=3|8;
	NVIC_EnableIRQ(TIMER1_IRQn);
	LPC_TIM1->TCR=1;
}


void app(void *arg){
	while(1){
		servo_A_lock(-90);
		servo_B_lock(90);
		osDelay(4000);
		servo_A_lock(90);
		servo_B_lock(-90);
		osDelay(4000);		
	}
	
}