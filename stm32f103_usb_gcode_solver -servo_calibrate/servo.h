#ifndef __SERVO_LPC_CH
#define __SERVO_LPC_CH
#include "gcode.h"
/*
	timer based servo contoler based on timer ablilty it can manage 3 servo motor
	pin mode is inverse it because of using not logic level shifter in output
*/

#define __servo_tmr_clk_init  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,RCC_APB1ENR_TIM4EN) 
#define __servo_tmr_struct 	TIM4
#define __servo_tmr_handler 	TIM4_IRQHandler
#define __servo_tmr_nvic_irq	TIM4_IRQn
#define __SERVO_TIMER_FREQ		RTE_PCLK1
#define __SERVO_COUNTER_FREQ	2000000


#define __servo_pin_struct_ch0   GPIOB 
#define __servo_clk_ch0_init		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,RCC_APB2ENR_IOPBEN)
#define __servo_pin_ch0				GPIO_Pin_0
#define __servo_pin_ch0_set	 	GPIOB->BSRR=1<<0
#define __servo_pin_ch0_clr	 	GPIOB->BRR=1<<0

#define __servo_pin_struct_ch1   GPIOB 
#define __servo_clk_ch1_init		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,RCC_APB2ENR_IOPBEN)
#define __servo_pin_ch1				GPIO_Pin_1
#define __servo_pin_ch1_set	 	GPIOB->BSRR=1<<1
#define __servo_pin_ch1_clr	 	GPIOB->BRR=1<<1

#define __servo_pin_struct_ch2   GPIOB 
#define __servo_clk_ch2_init		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,RCC_APB2ENR_IOPBEN)
#define __servo_pin_ch2				GPIO_Pin_2
#define __servo_pin_ch2_set	 	GPIOB->BSRR=1<<2
#define __servo_pin_ch2_clr	 	GPIOB->BRR=1<<2



extern void servoInit(void);
extern int  servoMove(int channel,int angle);
extern void servoOn(int channel);
extern void servoOff(int channel);


extern int servoGcInit(int ch);	
extern int servoGc(gCode *inp,int *angle);

#endif
