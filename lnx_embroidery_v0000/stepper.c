#include <LPC17xx.h>
#include "stepper.h"

static unsigned int 	_steps_ch0=0,_steps_ch1=0,_steps_ch2=0;
static _Bool 			_hold_en_ch0,_hold_en_ch1,_hold_en_ch2;

_Bool	stepperUpdate(void){
	_Bool flg=0;
	if(_steps_ch0){
		_steps_ch0-=1;
		if(_steps_ch0&1)	_STEPPER_CH0_STP_PIN_SET;
		else					_STEPPER_CH0_STP_PIN_CLR;
		if(_steps_ch0==0 && (!_hold_en_ch0)) _STEPPER_CH0_EN_PIN_SET;
		flg=1;
	}

	if(_steps_ch1){
		_steps_ch1-=1;
		if(_steps_ch1&1)	_STEPPER_CH1_STP_PIN_SET;
		else					_STEPPER_CH1_STP_PIN_CLR;
		if(_steps_ch1==0 && (!_hold_en_ch1)) _STEPPER_CH1_EN_PIN_SET;
		flg=1;
	}

	if(_steps_ch2){
		_steps_ch2-=1;
		if(_steps_ch2&1)	_STEPPER_CH2_STP_PIN_SET;
		else					_STEPPER_CH2_STP_PIN_CLR;
		if(_steps_ch2==0 && (!_hold_en_ch2)) _STEPPER_CH2_EN_PIN_SET;
		flg=1;
	}	
	return flg;
}

unsigned int stepperMove(int channel,unsigned int steps,int dirction){
	if(stepperIsBusy(channel))	return 0;
	if(steps==0) 					return 0;
	switch(channel){
		case 0:
			if(dirction==1)	_STEPPER_CH0_DIR_PIN_SET;
		   else					_STEPPER_CH0_DIR_PIN_CLR;
			if(_hold_en_ch0)	_STEPPER_CH0_EN_PIN_CLR;	
			_steps_ch0=steps<<1;
			return _steps_ch0;  
		case 1:
			if(dirction==1)	_STEPPER_CH1_DIR_PIN_SET;
		   else					_STEPPER_CH1_DIR_PIN_CLR;
			if(_hold_en_ch1)	_STEPPER_CH1_EN_PIN_CLR;
			_steps_ch1=steps<<1;
			return _steps_ch1;
		case 2:
			if(dirction==1)	_STEPPER_CH2_DIR_PIN_SET;
		   else					_STEPPER_CH2_DIR_PIN_CLR;
			if(_hold_en_ch2)	_STEPPER_CH2_EN_PIN_CLR;			
			_steps_ch2=steps<<1;
			return _steps_ch2;
	}	
	return 0;
}


unsigned int stepperIsBusy(int channel){
	switch(channel){
		case 0:
			return _steps_ch0;  
		case 1:
			return _steps_ch1;
		case 2:
			return _steps_ch2;
	}
	return 0;
}

void stepperConfig(int channel,_Bool hold_en){
	switch(channel){
		case 0:
			_STEPPER_CH0_DIR_PIN_INIT;
			_STEPPER_CH0_EN_PIN_INIT;
			_STEPPER_CH0_STP_PIN_INIT;
			
			_STEPPER_CH0_DIR_PIN_CLR;
			_STEPPER_CH0_STP_PIN_CLR;
			_hold_en_ch0=hold_en;
			_steps_ch0=0;
			if(_hold_en_ch0)	_STEPPER_CH0_EN_PIN_CLR;
			else 					_STEPPER_CH0_EN_PIN_SET;	
		break;
		case 1:
			_STEPPER_CH1_DIR_PIN_INIT;
			_STEPPER_CH1_EN_PIN_INIT;
			_STEPPER_CH1_STP_PIN_INIT;
			
			_STEPPER_CH1_DIR_PIN_CLR;
			_STEPPER_CH1_STP_PIN_CLR;
			_hold_en_ch1=hold_en;
			_steps_ch1=0;
			if(_hold_en_ch1)	_STEPPER_CH1_EN_PIN_CLR;
			else 					_STEPPER_CH1_EN_PIN_SET;	
		break;
		case 2:
			_STEPPER_CH2_DIR_PIN_INIT;
			_STEPPER_CH2_EN_PIN_INIT;
			_STEPPER_CH2_STP_PIN_INIT;
			
			_STEPPER_CH2_DIR_PIN_CLR;
			_STEPPER_CH2_STP_PIN_CLR;
			_hold_en_ch2=hold_en;
			_steps_ch2=0;
			if(_hold_en_ch2)	_STEPPER_CH2_EN_PIN_CLR;
			else 					_STEPPER_CH2_EN_PIN_SET;	
		break;		
	}
}
