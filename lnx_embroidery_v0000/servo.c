#include "servo.h"
#include "LPC17xx.h"

static _Bool ch0_en=0,ch1_en=0,ch2_en=0;

void __servo_tmr_handler(void){
	if(__servo_tmr_struct->IR & 1){
		if(ch0_en) __servo_pin_ch0_clr;
		if(ch1_en) __servo_pin_ch1_clr;
		if(ch2_en) __servo_pin_ch2_clr;
		__servo_tmr_struct->IR=1;
	}
	
	if(__servo_tmr_struct->IR & 2){
		if(ch0_en) __servo_pin_ch0_set;
		__servo_tmr_struct->IR=2;
	}

	if(__servo_tmr_struct->IR & 4){
		if(ch1_en) __servo_pin_ch1_set;
		__servo_tmr_struct->IR=4;
	}
	
	if(__servo_tmr_struct->IR & 8){
		if(ch2_en) __servo_pin_ch2_set;
		__servo_tmr_struct->IR=8;
	}
}


static unsigned int angle2MR(int angle){
	return (1500-2000*angle/180)*25;
	//return (unsigned int)(((__SERVO_TIMER_FREQ*15)/10000)-((__SERVO_TIMER_FREQ*2*angle)/(180*1000))); //1.5 ms - angle*2ms/180 || angle bigger than -90
}


void servoInit(void){
	__servo_tmr_struct->TCR=2;
	__servo_tmr_struct->PR=0;
	__servo_tmr_struct->MR0=__SERVO_TIMER_FREQ/50;
	__servo_tmr_struct->MCR=3;
	__servo_tmr_struct->IR=0xf;	
	NVIC_EnableIRQ(__servo_tmr_nvic_irq);
	ch0_en=0;
	ch1_en=0;
	ch2_en=0;	
	__servo_tmr_struct->TCR=1;
	
}

void servoOn(int channel){
	switch(channel){
		case 0:
			__servo_pin_ch0_init;
			__servo_pin_ch0_set;
		   __servo_tmr_struct->MR1=angle2MR(-90);
		   __servo_tmr_struct->MCR|=1<<3;
		   ch0_en=1;
		break;
		case 1:
			__servo_pin_ch1_init;
			__servo_pin_ch1_set;
		   __servo_tmr_struct->MR2=angle2MR(-90);
		   __servo_tmr_struct->MCR|=1<<6;
		   ch1_en=1;
		break;	
		case 2:
			__servo_pin_ch2_init;
			__servo_pin_ch2_set;
		   __servo_tmr_struct->MR3=angle2MR(-90);
		   __servo_tmr_struct->MCR|=1<<9;
		   ch2_en=1;
		break;		
	}	
}

int  servoMove(int channel,int angle){
	if(angle<-90) angle=-90;
	if(angle>90)  angle=90;
	
	switch(channel){
		case 0:
			if(ch0_en==0) return 0;
		   __servo_tmr_struct->MR1=angle2MR(angle);
			return 1;
		case 1:
			if(ch1_en==0) return 0;
		   __servo_tmr_struct->MR2=angle2MR(angle);
		   return 1;	
		case 2:
			if(ch2_en==0) return 0;
		   __servo_tmr_struct->MR3=angle2MR(angle);
		   return 1;
	}	
	return 0;
}


void servoOff(int channel){
	switch(channel){
			case 0:
				ch0_en=0;
			   __servo_pin_ch0_set;
				__servo_tmr_struct->MCR&=~(unsigned int)(1<<3);

			break;
			case 1:
				ch1_en=0;
			   __servo_pin_ch1_set;
				__servo_tmr_struct->MCR&=~(unsigned int)(1<<6);

			break;	
			case 2:
				ch2_en=0;
				__servo_pin_ch2_set;
				__servo_tmr_struct->MCR&=~(unsigned int)(1<<9);
			break;		
		}	
}
