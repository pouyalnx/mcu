#ifndef __SERVO_LPC_CH
#define __SERVO_LPC_CH


#define __servo_tmr_struct 	LPC_TIM1
#define __servo_tmr_handler 	TIMER1_IRQHandler
#define __servo_tmr_nvic_irq	TIMER1_IRQn
#define __SERVO_TIMER_FREQ		25000000	

#define __servo_pin_ch0_init	 LPC_GPIO2->FIODIR|=1<<7
#define __servo_pin_ch0_set	 LPC_GPIO2->FIOSET=1<<7
#define __servo_pin_ch0_clr	 LPC_GPIO2->FIOCLR=1<<7

#define __servo_pin_ch1_init	 LPC_GPIO2->FIODIR|=2
#define __servo_pin_ch1_set	 LPC_GPIO2->FIOSET=2
#define __servo_pin_ch1_clr	 LPC_GPIO2->FIOCLR=2

#define __servo_pin_ch2_init	 LPC_GPIO2->FIODIR|=4
#define __servo_pin_ch2_set	 LPC_GPIO2->FIOSET=4
#define __servo_pin_ch2_clr	 LPC_GPIO2->FIOCLR=4

extern void servoInit(void);
extern int  servoMove(int channel,int angle);
extern void servoOn(int channel);
extern void servoOff(int channel);



#endif
