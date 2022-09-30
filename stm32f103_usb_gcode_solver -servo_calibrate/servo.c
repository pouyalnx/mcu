#include "servo.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "RTE_Device.h"
#include "cmsis_os.h"

static _Bool ch0_en=0,ch1_en=0,ch2_en=0;

void __servo_tmr_handler(void){
	if(__servo_tmr_struct->SR & TIM_SR_UIF){
		if(ch0_en) __servo_pin_ch0_clr;
		if(ch1_en) __servo_pin_ch1_clr;
		if(ch2_en) __servo_pin_ch2_clr;
		__servo_tmr_struct->SR&=~TIM_SR_UIF;
	}
	
	if(__servo_tmr_struct->SR & TIM_SR_CC1IF){
		if(ch0_en) __servo_pin_ch0_set;
		__servo_tmr_struct->SR&=~TIM_SR_CC1IF;
	}

	if(__servo_tmr_struct->SR & TIM_SR_CC2IF){
		if(ch1_en) __servo_pin_ch1_set;
		__servo_tmr_struct->SR&=~TIM_SR_CC2IF;
	}
	
	if(__servo_tmr_struct->SR & TIM_SR_CC3IF){
		if(ch2_en) __servo_pin_ch2_set;
		__servo_tmr_struct->SR&=~TIM_SR_CC3IF;
	}
}

static int angeltoUs(int angle){
	if(angle<0)		angle=0;
	if(angle>180)	angle=180;
	//	0-->1000 us
	// 1800---> 4600 us linear estimate
	return 20*angle+1000;
}

static uint16_t us2tmrccr(int us){
	return  (uint16_t)(us*__SERVO_COUNTER_FREQ/1000000);//(((__SERVO_COUNTER_FREQ*15)/10000)-((__SERVO_COUNTER_FREQ*2*angle)/(180*1000))); //1.5 ms - angle*2ms/180 || angle bigger than -90
}


static uint16_t angle2MR(int angle){
	return us2tmrccr(angeltoUs(angle));
}

void servoInit(void){
	TIM_TimeBaseInitTypeDef tmr;
	__servo_tmr_clk_init;
	TIM_TimeBaseStructInit(&tmr);
	tmr.TIM_CounterMode=TIM_CounterMode_Up;
	tmr.TIM_Prescaler=__SERVO_TIMER_FREQ/__SERVO_COUNTER_FREQ;
	tmr.TIM_Period=(__SERVO_COUNTER_FREQ/1000)*20; //20 ms for timer
	TIM_TimeBaseInit(__servo_tmr_struct,&tmr);
	
	TIM_OCInitTypeDef oc;
	TIM_OCStructInit(&oc);
	oc.TIM_OCMode=TIM_OCMode_Timing;
	oc.TIM_Pulse=angle2MR(0);
	TIM_OC1Init(__servo_tmr_struct,&oc);

	TIM_OCStructInit(&oc);
	oc.TIM_OCMode=TIM_OCMode_Timing;
	oc.TIM_Pulse=angle2MR(0);
	TIM_OC2Init(__servo_tmr_struct,&oc);

	TIM_OCStructInit(&oc);
	oc.TIM_OCMode=TIM_OCMode_Timing;
	oc.TIM_Pulse=angle2MR(0);
	TIM_OC3Init(__servo_tmr_struct,&oc);	
	
	TIM_ClearFlag(__servo_tmr_struct,TIM_FLAG_CC1);
	TIM_ClearFlag(__servo_tmr_struct,TIM_FLAG_CC2);
	TIM_ClearFlag(__servo_tmr_struct,TIM_FLAG_CC3);
	
	TIM_ClearFlag(__servo_tmr_struct,TIM_FLAG_Update);
	TIM_ITConfig(__servo_tmr_struct,TIM_IT_Update,ENABLE);
	NVIC_EnableIRQ(__servo_tmr_nvic_irq);
	ch0_en=0;
	ch1_en=0;
	ch2_en=0;	
	TIM_Cmd(__servo_tmr_struct,ENABLE);
	
}

void servoOn(int channel){
	GPIO_InitTypeDef pin;
	switch(channel){
		case 0:
			__servo_clk_ch0_init;
			pin.GPIO_Mode=GPIO_Mode_Out_PP;
			pin.GPIO_Pin=__servo_pin_ch0;
			pin.GPIO_Speed=GPIO_Speed_50MHz;
			GPIO_Init(__servo_pin_struct_ch0,&pin);
			__servo_pin_ch0_set;
		
			TIM_SetCompare1(__servo_tmr_struct,angle2MR(0));
			TIM_ClearFlag(__servo_tmr_struct,TIM_FLAG_CC1);
			TIM_ITConfig(__servo_tmr_struct,TIM_IT_CC1,ENABLE);

		   ch0_en=1;
		break;
		case 1:
			__servo_clk_ch1_init;
			pin.GPIO_Mode=GPIO_Mode_Out_PP;
			pin.GPIO_Pin=__servo_pin_ch1;
			pin.GPIO_Speed=GPIO_Speed_50MHz;
			GPIO_Init(__servo_pin_struct_ch1,&pin);
			__servo_pin_ch1_set;
		
			TIM_SetCompare2(__servo_tmr_struct,angle2MR(0));
			TIM_ClearFlag(__servo_tmr_struct,TIM_FLAG_CC2);
			TIM_ITConfig(__servo_tmr_struct,TIM_IT_CC2,ENABLE);
		   ch1_en=1;
		break;	
		case 2:
			__servo_clk_ch2_init;
			pin.GPIO_Mode=GPIO_Mode_Out_PP;
			pin.GPIO_Pin=__servo_pin_ch2;
			pin.GPIO_Speed=GPIO_Speed_50MHz;
			GPIO_Init(__servo_pin_struct_ch2,&pin);
			__servo_pin_ch0_set;
		
			TIM_SetCompare1(__servo_tmr_struct,angle2MR(0));
			TIM_ClearFlag(__servo_tmr_struct,TIM_FLAG_CC3);
			TIM_ITConfig(__servo_tmr_struct,TIM_IT_CC3,ENABLE);
		   ch2_en=1;
		break;		
	}	
}

int  servoMove(int channel,int angle){
	//if(angle<-90) angle=-90;
	//if(angle>90)  angle=90;
	
	switch(channel){
		case 0:
			if(ch0_en==0) return 0;
		   TIM_SetCompare1(__servo_tmr_struct,angle2MR(angle));
			return 1;
		case 1:
			if(ch1_en==0) return 0;
		   TIM_SetCompare2(__servo_tmr_struct,angle2MR(angle));
		   return 1;	
		case 2:
			if(ch2_en==0) return 0;
		   TIM_SetCompare3(__servo_tmr_struct,angle2MR(angle));
		   return 1;
	}	
	return 0;
}


void servoOff(int channel){
	switch(channel){
			case 0:
				ch0_en=0;
			   __servo_pin_ch0_set;
				TIM_ITConfig(__servo_tmr_struct,TIM_IT_CC1,DISABLE);

			break;
			case 1:
				ch1_en=0;
			   __servo_pin_ch1_set;
				TIM_ITConfig(__servo_tmr_struct,TIM_IT_CC2,DISABLE);

			break;	
			case 2:
				ch2_en=0;
				__servo_pin_ch2_set;
				TIM_ITConfig(__servo_tmr_struct,TIM_IT_CC3,DISABLE);
			break;		
		}	
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
static int _servoGc_ch;
static int _servoGc_m3_angel;
static int _servoGc_m5_angel;
static int _servoGc_angel;

int servoGcInit(int ch){
	_servoGc_ch=ch;
	servoInit();
	servoOn(_servoGc_ch);
	_servoGc_m3_angel=90;
	_servoGc_m5_angel=-90;
	_servoGc_angel=90;
	servoMove(_servoGc_ch,_servoGc_angel);
	return GCODE_OK;
}

int servoGc(gCode *inp,int *angel){
	if(angel!=0)	*angel=_servoGc_angel;	
	if(inp->cword=='M' && inp->ccode==3){
		servoMove(_servoGc_ch,_servoGc_m3_angel);
		osDelay(1000);
		_servoGc_angel=_servoGc_m3_angel;
		if(angel!=0)	*angel=_servoGc_angel;
		return GCODE_OK;
	}
	
	if(inp->cword=='M' && inp->ccode==5){
		servoMove(_servoGc_ch,_servoGc_m5_angel);
		osDelay(1000);
		_servoGc_angel=_servoGc_m5_angel;
		if(angel!=0)	*angel=_servoGc_angel;
		return GCODE_OK;
	}

	if(inp->cword=='M' && inp->ccode==30 && inp->key[gArg('A')]){
		_servoGc_m3_angel=(int)inp->value[gArg('A')];
		servoMove(_servoGc_ch,_servoGc_m3_angel);
		osDelay(1000);
		_servoGc_angel=_servoGc_m3_angel;
		if(angel!=0)	*angel=_servoGc_angel;
		return GCODE_OK;
	}
		
	if(inp->cword=='M' && inp->ccode==50 && inp->key[gArg('A')]){
		_servoGc_m5_angel=(int)inp->value[gArg('A')];
		servoMove(_servoGc_ch,_servoGc_m5_angel);
		osDelay(1000);
		_servoGc_angel=_servoGc_m5_angel;
		if(angel!=0)	*angel=_servoGc_angel;
		return GCODE_OK;
	}	
	
	return GCODE_ERROR_UNSUPPORTED_CMD;
}






