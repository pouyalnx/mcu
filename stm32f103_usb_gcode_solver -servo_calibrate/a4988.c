#include "a4988.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "RTE_Device.h"
#include "math.h"
///////////////////////////////////////////////////////////////////////////////
//	delay functions that used for all core functions
///////////////////////////////////////////////////////////////////////////////
#include "cmsis_os.h"
static void coreDelay(unsigned int ms){
	osDelay(ms);
}
///////////////////////////////////////////////////////////////////////////////
//	port status varibles and static memory
///////////////////////////////////////////////////////////////////////////////
static int ch0_mode;
static int ch0_dir;
static int ch0_tick;
static int ch0_stat;
static int ch0_features;
#ifdef __A4988_CH0_ROTATE_SENSOR
static int ch0_rotate_sensor_stat;
#endif


static int ch1_mode;
static int ch1_dir;
static int ch1_tick;
static int ch1_stat;
static int ch1_features;
#ifdef __A4988_CH1_ROTATE_SENSOR
static int ch1_rotate_sensor_stat;
#endif


static int ch10_mode;
static int ch10_dir;
static int ch10_tick;
static int ch10_stat;
static int ch10_features;
#ifdef __A4988_CH10_ROTATE_SENSOR
static int ch10_rotate_sensor_stat;
#endif


static int ch20_mode;
static int ch20_dir;
static int ch20_tick;
static int ch20_stat;
static int ch20_features;
#ifdef __A4988_CH20_ROTATE_SENSOR
static int ch20_rotate_sensor_stat;
#endif

///////////////////////////////////////////////////////////////////////////////
//	Extra Utils Linear Move
///////////////////////////////////////////////////////////////////////////////
int linearMoveToStep(double mm,int a4988_mode,int motor_step_per_revolution,int belt_pich_mm,int poly_tooth_count){
	double ans=(double)(mm*motor_step_per_revolution*a4988_mode)/(double)(belt_pich_mm*poly_tooth_count);
	if(ans<0) return -(int)(-ans+0.5);
	return (int)(ans+0.5);
}


double stepToLinearMove(int step,int a4988_mode,int motor_step_per_revolution,int belt_pich_mm,int poly_tooth_count){
	return ((double)(step*belt_pich_mm*poly_tooth_count))/(double)(motor_step_per_revolution*a4988_mode);
}


int linearMoveToStepDefault(double mm){
	return linearMoveToStep(mm,A4988_MODE_QUARTERSTEP,MOTOR_STEP_PER_REVOLUTION_200,BELT_PITCH_IN_MM_2_GT2_DEFAULT,POLLY_TOOTH_COUNT_20_DEFAULT);
}


double stepToLinearMoveDefault(int step){
	return stepToLinearMove(step,A4988_MODE_QUARTERSTEP,MOTOR_STEP_PER_REVOLUTION_200,BELT_PITCH_IN_MM_2_GT2_DEFAULT,POLLY_TOOTH_COUNT_20_DEFAULT);
}


/////////////////////////////////////////////////////////////////////////////////
//Extra Utils Rotate Mode
/////////////////////////////////////////////////////////////////////////////////
int angleRotateToStep(double deg,int a4988_mode,int motor_step_per_revolution){
	double ans=(double)(deg*motor_step_per_revolution*a4988_mode)/(360.0);
	if(ans<0) return -(int)(-ans+0.5);
	return (int)(ans+0.5);
}

int angleRotateToStepDefault(double deg){
	return angleRotateToStep(deg,A4988_MODE_QUARTERSTEP,MOTOR_STEP_PER_REVOLUTION_200);
}


double stepToAngleRotate(int step,int a4988_mode,int motor_step_per_revolution){
	return ((double)step*360.0)/(double)(motor_step_per_revolution*a4988_mode);
}

double stepToangleRotateDefault(int step){
	return stepToAngleRotate(step,A4988_MODE_QUARTERSTEP,MOTOR_STEP_PER_REVOLUTION_200);
}

///////////////////////////////////////////////////////////////////////////////
//timer contol functions
///////////////////////////////////////////////////////////////////////////////
void __A4988_TMR0_IRQHandler(void){
	__A4988_TMR0->SR=0;
	if(ch0_stat==0 && ch0_tick>0){
		ch0_tick-=1;
		
		#ifdef __A4988_CH0_LM_HIGH_SENSOR
		if(ch0_dir==1 && __A4988_CH0_LM_HIGH_SENSOR_TRIGRED){ ch0_stat=A4988_BUSY_STAT_ERROR_HIGH_SENSOR; return;}
		#endif
		#ifdef __A4988_CH0_LM_LOW_SENSOR
		if(ch0_dir==0 && __A4988_CH0_LM_LOW_SENSOR_TRIGRED){ ch0_stat=A4988_BUSY_STAT_ERROR_LOW_SENSOR; return;}
		#endif
		#ifdef __A4988_CH0_ROTATE_SENSOR
		if(ch0_rotate_sensor_stat==0 && (__A4988_CH0_ROTATE_SENSOR_READ)>0){
			ch0_stat=A4988_BUSY_STAT_ERROR_ROT_SENSOR;
			ch0_rotate_sensor_stat=__A4988_CH0_ROTATE_SENSOR_READ;
			return;
		}		
		ch0_rotate_sensor_stat=__A4988_CH0_ROTATE_SENSOR_READ;
		#endif		
		
		
		if(ch0_tick&1){__A4988_CH0_STP_CLR;}
		else{__A4988_CH0_STP_SET;}
	}

	
	if(ch1_stat==0 && ch1_tick>0){
		ch1_tick-=1;
		
		#ifdef __A4988_CH1_LM_HIGH_SENSOR
		if(ch1_dir==1 && __A4988_CH1_LM_HIGH_SENSOR_TRIGRED){ ch1_stat=A4988_BUSY_STAT_ERROR_HIGH_SENSOR; return;}
		#endif
		#ifdef __A4988_CH1_LM_LOW_SENSOR
		if(ch1_dir==0 && __A4988_CH1_LM_LOW_SENSOR_TRIGRED){ ch1_stat=A4988_BUSY_STAT_ERROR_LOW_SENSOR; return;}
		#endif
		#ifdef __A4988_CH1_ROTATE_SENSOR
		if(ch1_rotate_sensor_stat==0 && (__A4988_CH1_ROTATE_SENSOR_READ)>0){
			ch1_stat=A4988_BUSY_STAT_ERROR_ROT_SENSOR;
			ch1_rotate_sensor_stat=__A4988_CH1_ROTATE_SENSOR_READ;
			return;
		}		
		ch1_rotate_sensor_stat=__A4988_CH1_ROTATE_SENSOR_READ;
		#endif		
		
		
		if(ch1_tick&1){__A4988_CH1_STP_CLR;}
		else{__A4988_CH1_STP_SET;}
	}
	
}

void __A4988_TMR1_IRQHandler(void){
	if(ch10_stat==0 && ch10_tick>0){
		ch10_tick-=1;
		
		#ifdef __A4988_CH10_LM_HIGH_SENSOR
		if(ch10_dir==1 && __A4988_CH10_LM_HIGH_SENSOR_TRIGRED){ ch10_stat=A4988_BUSY_STAT_ERROR_HIGH_SENSOR; return;}
		#endif
		#ifdef __A4988_CH10_LM_LOW_SENSOR
		if(ch10_dir==0 && __A4988_CH10_LM_LOW_SENSOR_TRIGRED){ ch10_stat=A4988_BUSY_STAT_ERROR_LOW_SENSOR; return;}
		#endif
		#ifdef __A4988_CH0_ROTATE_SENSOR
		if(ch10_rotate_sensor_stat==0 && (__A4988_CH10_ROTATE_SENSOR_READ)>0){
			ch10_stat=A4988_BUSY_STAT_ERROR_ROT_SENSOR;
			ch10_rotate_sensor_stat=__A4988_CH10_ROTATE_SENSOR_READ;
			return;
		}		
		ch10_rotate_sensor_stat=__A4988_CH10_ROTATE_SENSOR_READ;
		#endif		
		
		
		if(ch10_tick&1){__A4988_CH10_STP_CLR;}
		else{__A4988_CH10_STP_SET;}
	}	
	__A4988_TMR1->SR=0;
	
}

void __A4988_TMR2_IRQHandler(void){
	if(ch20_stat==0 && ch20_tick>0){
		ch20_tick-=1;
		
		#ifdef __A4988_CH20_LM_HIGH_SENSOR
		if(ch20_dir==1 && __A4988_CH20_LM_HIGH_SENSOR_TRIGRED){ ch20_stat=A4988_BUSY_STAT_ERROR_HIGH_SENSOR; return;}
		#endif
		#ifdef __A4988_CH20_LM_LOW_SENSOR
		if(ch20_dir==0 && __A4988_CH20_LM_LOW_SENSOR_TRIGRED){ ch20_stat=A4988_BUSY_STAT_ERROR_LOW_SENSOR; return;}
		#endif
		#ifdef __A4988_CH0_ROTATE_SENSOR
		if(ch20_rotate_sensor_stat==0 && (__A4988_CH20_ROTATE_SENSOR_READ)>0){
			ch20_stat=A4988_BUSY_STAT_ERROR_ROT_SENSOR;
			ch20_rotate_sensor_stat=__A4988_CH0_ROTATE_SENSOR_READ;
			return;
		}		
		ch20_rotate_sensor_stat=__A4988_CH20_ROTATE_SENSOR_READ;
		#endif		
		
		
		if(ch20_tick&1){__A4988_CH20_STP_CLR;}
		else{__A4988_CH20_STP_SET;}
	}	
	__A4988_TMR2->SR=0;
}
///////////////////////////////////////////////////////////////////////////////
// TimeUnit 0//////////////////////////////////////////////////////////////////

int a4988Tmr0Init(int delay_us){
	RCC_ClocksTypeDef clk;
	int period;
	if(delay_us<0)												return -1;
	period=(delay_us*__A4988_TMR0_FREQUENCY)/1000000;
	if(period>65535 || period<0)							return -2;
	
	TIM_TimeBaseInitTypeDef tmr;
	TIM_TimeBaseStructInit(&tmr);
	tmr.TIM_ClockDivision=0;
	tmr.TIM_CounterMode=TIM_CounterMode_Up;
	RCC_GetClocksFreq(&clk);
	tmr.TIM_Prescaler=(clk.__A4988_TMR0_INPUT_FREQ/__A4988_TMR0_FREQUENCY)-1;
	tmr.TIM_Period=(uint16_t)period;
	__A4988_TMR0_CLK_ENABLE;
	TIM_TimeBaseInit(__A4988_TMR0,&tmr);
	TIM_ClearFlag(__A4988_TMR0,TIM_FLAG_Update);
	TIM_ITConfig(__A4988_TMR0,TIM_IT_Update,ENABLE);
	NVIC_EnableIRQ(__A4988_TMR0_IRQn);
	TIM_Cmd(__A4988_TMR0,ENABLE);
	return A4988_OK;
}

void a4988Tmr0DeInit(void){
	NVIC_DisableIRQ(__A4988_TMR0_IRQn);
	TIM_Cmd(__A4988_TMR0,DISABLE);	
}
int a4988Tmr0GetDelay(void){
	return (((uint32_t)__A4988_TMR0->ARR)*1000000)/__A4988_TMR0_FREQUENCY;
}
int a4988Tmr0SetDelay(int delay_us){
	int period;
	if(delay_us<0)												return -1;
	period=delay_us*__A4988_TMR0_FREQUENCY/1000000;
	if(period>65535 || period<0)							return -2;
	TIM_SetAutoreload(__A4988_TMR0,(uint16_t)period);
	__A4988_TMR0->CNT=0;
	return A4988_OK;
}
///////////////////////////////////////////////////////////////////////////////
//	timeunit 1//////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int a4988Tmr1Init(int delay_us){
	RCC_ClocksTypeDef clk;
	int period;
	if(delay_us<0)												return -1;
	period=(delay_us*__A4988_TMR1_FREQUENCY)/1000000;
	if(period>65535 || period<0)							return -2;
	
	TIM_TimeBaseInitTypeDef tmr;
	TIM_TimeBaseStructInit(&tmr);
	tmr.TIM_ClockDivision=0;
	tmr.TIM_CounterMode=TIM_CounterMode_Up;
	RCC_GetClocksFreq(&clk);
	tmr.TIM_Prescaler=(clk.__A4988_TMR1_INPUT_FREQ/__A4988_TMR1_FREQUENCY)-1;
	tmr.TIM_Period=(uint16_t)period;
	__A4988_TMR1_CLK_ENABLE;
	TIM_TimeBaseInit(__A4988_TMR1,&tmr);
	TIM_ClearFlag(__A4988_TMR1,TIM_FLAG_Update);
	TIM_ITConfig(__A4988_TMR1,TIM_IT_Update,ENABLE);
	NVIC_EnableIRQ(__A4988_TMR1_IRQn);
	TIM_Cmd(__A4988_TMR1,ENABLE);
	return A4988_OK;
}

void a4988Tmr1DeInit(void){
	NVIC_DisableIRQ(__A4988_TMR1_IRQn);
	TIM_Cmd(__A4988_TMR1,DISABLE);	
}
int a4988Tmr1GetDelay(void){
	return (((uint32_t)__A4988_TMR1->ARR)*1000000)/__A4988_TMR1_FREQUENCY;
}
int a4988Tmr1SetDelay(int delay_us){
	int period;
	if(delay_us<0)												return -1;
	period=delay_us*__A4988_TMR1_FREQUENCY/1000000;
	if(period>65535 || period<0)							return -2;
	TIM_SetAutoreload(__A4988_TMR1,(uint16_t)period);
	__A4988_TMR1->CNT=0;
	return A4988_OK;
}
///////////////////////////////////////////////////////////////////////////////
// Time2 Unit//////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int a4988Tmr2Init(int delay_us){
	RCC_ClocksTypeDef clk;
	int period;
	if(delay_us<0)												return -1;
	period=(delay_us*__A4988_TMR2_FREQUENCY)/1000000;
	if(period>65535 || period<0)							return -2;
	
	TIM_TimeBaseInitTypeDef tmr;
	TIM_TimeBaseStructInit(&tmr);
	tmr.TIM_ClockDivision=0;
	tmr.TIM_CounterMode=TIM_CounterMode_Up;
	RCC_GetClocksFreq(&clk);
	tmr.TIM_Prescaler=(clk.__A4988_TMR2_INPUT_FREQ/__A4988_TMR2_FREQUENCY)-1;
	tmr.TIM_Period=(uint16_t)period;
	__A4988_TMR2_CLK_ENABLE;
	TIM_TimeBaseInit(__A4988_TMR2,&tmr);
	TIM_ClearFlag(__A4988_TMR2,TIM_FLAG_Update);
	TIM_ITConfig(__A4988_TMR2,TIM_IT_Update,ENABLE);
	NVIC_EnableIRQ(__A4988_TMR2_IRQn);
	TIM_Cmd(__A4988_TMR2,ENABLE);
	return A4988_OK;
}

void a4988Tmr2DeInit(void){
	NVIC_DisableIRQ(__A4988_TMR2_IRQn);
	TIM_Cmd(__A4988_TMR2,DISABLE);	
}
int a4988Tmr2GetDelay(void){
	return (((uint32_t)__A4988_TMR2->ARR)*1000000)/__A4988_TMR2_FREQUENCY;
}
int a4988Tmr2SetDelay(int delay_us){
	int period;
	if(delay_us<0)												return -1;
	period=delay_us*__A4988_TMR2_FREQUENCY/1000000;
	if(period>65535 || period<0)							return -2;
	TIM_SetAutoreload(__A4988_TMR2,(uint16_t)period);
	__A4988_TMR2->CNT=0;
	return A4988_OK;
}
///////////////////////////////////////////////////////////////////////////////
//channel contol functions
///////////////////////////////////////////////////////////////////////////////
int a4899ChInit(int ch,int mode){
	GPIO_InitTypeDef gpio;
	if(ch==__A4988_CH0){
		ch0_features=0;
		__A4988_CH0_DIR_CLK_ENABLE;
		__A4988_CH0_EN_CLK_ENABLE;
		__A4988_CH0_STP_CLK_ENABLE;

		gpio.GPIO_Mode=GPIO_Mode_Out_PP;
		gpio.GPIO_Speed=GPIO_Speed_50MHz;
		
		gpio.GPIO_Pin=__A4988_CH0_DIR_PIN;
		GPIO_Init(__A4988_CH0_DIR_PORT,&gpio);

		gpio.GPIO_Pin=__A4988_CH0_EN_PIN;
		GPIO_Init(__A4988_CH0_EN_PORT,&gpio);

		gpio.GPIO_Pin=__A4988_CH0_STP_PIN;
		GPIO_Init(__A4988_CH0_STP_PORT,&gpio);	

		ch0_dir=1;
		__A4988_CH0_DIR_SET;
		ch0_tick=0;
		ch0_stat=0;
		
		gpio.GPIO_Mode=GPIO_Mode_IPD;
		
		#ifdef __A4988_CH0_LM_HIGH_SENSOR
		__A4988_CH0_LM_HIGH_SENSOR_CLK_ENABLE;
		gpio.GPIO_Pin=__A4988_CH0_LM_HIGH_SENSOR_PIN;
		GPIO_Init(__A4988_CH0_LM_HIGH_SENSOR_PORT,&gpio);
		ch0_features|=A4988_FEATURE_HIGH_SENSOR;
		#endif

		#ifdef __A4988_CH0_LM_LOW_SENSOR
		__A4988_CH0_LM_LOW_SENSOR_CLK_ENABLE;
		gpio.GPIO_Pin=__A4988_CH0_LM_LOW_SENSOR_PIN;
		GPIO_Init(__A4988_CH0_LM_LOW_SENSOR_PORT,&gpio);
		ch0_features|=A4988_FEATURE_LOW_SENSOR;		
		#endif


		#ifdef __A4988_CH0_ROTATE_SENSOR
		__A4988_CH0_ROTATE_SENSOR_CLK_ENABLE;
		gpio.GPIO_Pin=__A4988_CH0_ROTATE_SENSOR_PIN;
		GPIO_Init(__A4988_CH0_ROTATE_SENSOR_PORT,&gpio);			
		ch0_rotate_sensor_stat=__A4988_CH0_ROTATE_SENSOR_READ;
		ch0_features|=A4988_FEATURE_ROTATE_SENSOR;
		#endif

		a4988chEnable(ch);
		return a4988chSetMode(ch,mode);
	}
	
	if(ch==__A4988_CH1){
		ch1_features=0;
		__A4988_CH1_DIR_CLK_ENABLE;
		__A4988_CH1_EN_CLK_ENABLE;
		__A4988_CH1_STP_CLK_ENABLE;

		gpio.GPIO_Mode=GPIO_Mode_Out_PP;
		gpio.GPIO_Speed=GPIO_Speed_50MHz;
		
		gpio.GPIO_Pin=__A4988_CH1_DIR_PIN;
		GPIO_Init(__A4988_CH1_DIR_PORT,&gpio);

		gpio.GPIO_Pin=__A4988_CH1_EN_PIN;
		GPIO_Init(__A4988_CH1_EN_PORT,&gpio);

		gpio.GPIO_Pin=__A4988_CH1_STP_PIN;
		GPIO_Init(__A4988_CH1_STP_PORT,&gpio);	

		ch1_dir=1;
		__A4988_CH1_DIR_SET;
		ch1_tick=0;
		ch1_stat=0;
		
		gpio.GPIO_Mode=GPIO_Mode_IPD;
		
		#ifdef __A4988_CH1_LM_HIGH_SENSOR
		__A4988_CH1_LM_HIGH_SENSOR_CLK_ENABLE;
		gpio.GPIO_Pin=__A4988_CH1_LM_HIGH_SENSOR_PIN;
		GPIO_Init(__A4988_CH1_LM_HIGH_SENSOR_PORT,&gpio);
		ch1_features|=A4988_FEATURE_HIGH_SENSOR;
		#endif

		#ifdef __A4988_CH1_LM_LOW_SENSOR
		__A4988_CH1_LM_LOW_SENSOR_CLK_ENABLE;
		gpio.GPIO_Pin=__A4988_CH1_LM_LOW_SENSOR_PIN;
		GPIO_Init(__A4988_CH1_LM_LOW_SENSOR_PORT,&gpio);
		ch1_features|=A4988_FEATURE_LOW_SENSOR;		
		#endif


		#ifdef __A4988_CH1_ROTATE_SENSOR
		__A4988_CH1_ROTATE_SENSOR_CLK_ENABLE;
		gpio.GPIO_Pin=__A4988_CH1_ROTATE_SENSOR_PIN;
		GPIO_Init(__A4988_CH1_ROTATE_SENSOR_PORT,&gpio);			
		ch1_rotate_sensor_stat=__A4988_CH1_ROTATE_SENSOR_READ;
		ch1_features|=A4988_FEATURE_ROTATE_SENSOR;
		#endif

		a4988chEnable(ch);
		return a4988chSetMode(ch,mode);
	}	

	if(ch==__A4988_CH10){
		ch10_features=0;
		__A4988_CH10_DIR_CLK_ENABLE;
		__A4988_CH10_EN_CLK_ENABLE;
		__A4988_CH10_STP_CLK_ENABLE;

		gpio.GPIO_Mode=GPIO_Mode_Out_PP;
		gpio.GPIO_Speed=GPIO_Speed_50MHz;
		
		gpio.GPIO_Pin=__A4988_CH10_DIR_PIN;
		GPIO_Init(__A4988_CH10_DIR_PORT,&gpio);

		gpio.GPIO_Pin=__A4988_CH10_EN_PIN;
		GPIO_Init(__A4988_CH10_EN_PORT,&gpio);

		gpio.GPIO_Pin=__A4988_CH10_STP_PIN;
		GPIO_Init(__A4988_CH10_STP_PORT,&gpio);	

		ch10_dir=1;
		__A4988_CH10_DIR_SET;
		ch10_tick=0;
		ch10_stat=0;
		
		gpio.GPIO_Mode=GPIO_Mode_IPD;
		
		#ifdef __A4988_CH10_LM_HIGH_SENSOR
		__A4988_CH10_LM_HIGH_SENSOR_CLK_ENABLE;
		gpio.GPIO_Pin=__A4988_CH10_LM_HIGH_SENSOR_PIN;
		GPIO_Init(__A4988_C10_LM_HIGH_SENSOR_PORT,&gpio);
		ch10_features|=A4988_FEATURE_HIGH_SENSOR;
		#endif

		#ifdef __A4988_CH10_LM_LOW_SENSOR
		__A4988_CH10_LM_LOW_SENSOR_CLK_ENABLE;
		gpio.GPIO_Pin=__A4988_CH10_LM_LOW_SENSOR_PIN;
		GPIO_Init(__A4988_CH10_LM_LOW_SENSOR_PORT,&gpio);
		ch10_features|=A4988_FEATURE_LOW_SENSOR;		
		#endif


		#ifdef __A4988_CH10_ROTATE_SENSOR
		__A4988_CH10_ROTATE_SENSOR_CLK_ENABLE;
		gpio.GPIO_Pin=__A4988_CH10_ROTATE_SENSOR_PIN;
		GPIO_Init(__A4988_CH10_ROTATE_SENSOR_PORT,&gpio);			
		ch10_rotate_sensor_stat=__A4988_CH10_ROTATE_SENSOR_READ;
		ch10_features|=A4988_FEATURE_ROTATE_SENSOR;
		#endif

		a4988chEnable(ch);
		return a4988chSetMode(ch,mode);
	}
	
	if(ch==__A4988_CH20){
		ch20_features=0;
		__A4988_CH20_DIR_CLK_ENABLE;
		__A4988_CH20_EN_CLK_ENABLE;
		__A4988_CH20_STP_CLK_ENABLE;

		gpio.GPIO_Mode=GPIO_Mode_Out_PP;
		gpio.GPIO_Speed=GPIO_Speed_50MHz;
		
		gpio.GPIO_Pin=__A4988_CH20_DIR_PIN;
		GPIO_Init(__A4988_CH20_DIR_PORT,&gpio);

		gpio.GPIO_Pin=__A4988_CH20_EN_PIN;
		GPIO_Init(__A4988_CH20_EN_PORT,&gpio);

		gpio.GPIO_Pin=__A4988_CH20_STP_PIN;
		GPIO_Init(__A4988_CH20_STP_PORT,&gpio);	

		ch20_dir=1;
		__A4988_CH20_DIR_SET;
		ch20_tick=0;
		ch20_stat=0;
		
		gpio.GPIO_Mode=GPIO_Mode_IPD;
		
		#ifdef __A4988_CH20_LM_HIGH_SENSOR
		__A4988_CH20_LM_HIGH_SENSOR_CLK_ENABLE;
		gpio.GPIO_Pin=__A4988_CH20_LM_HIGH_SENSOR_PIN;
		GPIO_Init(__A4988_CH20_LM_HIGH_SENSOR_PORT,&gpio);
		ch20_features|=A4988_FEATURE_HIGH_SENSOR;
		#endif

		#ifdef __A4988_CH20_LM_LOW_SENSOR
		__A4988_CH20_LM_LOW_SENSOR_CLK_ENABLE;
		gpio.GPIO_Pin=__A4988_CH20_LM_LOW_SENSOR_PIN;
		GPIO_Init(__A4988_CH20_LM_LOW_SENSOR_PORT,&gpio);
		ch20_features|=A4988_FEATURE_LOW_SENSOR;		
		#endif


		#ifdef __A4988_CH20_ROTATE_SENSOR
		__A4988_CH20_ROTATE_SENSOR_CLK_ENABLE;
		gpio.GPIO_Pin=__A4988_CH20_ROTATE_SENSOR_PIN;
		GPIO_Init(__A4988_CH20_ROTATE_SENSOR_PORT,&gpio);			
		ch20_rotate_sensor_stat=__A4988_CH20_ROTATE_SENSOR_READ;
		ch20_features|=A4988_FEATURE_ROTATE_SENSOR;
		#endif

		a4988chEnable(ch);
		return a4988chSetMode(ch,mode);
	}	
	
	return A4988_ERROR_INVALID_CHANNEL;
}


int a4988chSetMode(int ch,int mode){
	if(ch==__A4988_CH0){
		if(mode==A4988_MODE_FULLSTEP){
			ch0_mode=mode;
			return A4988_OK;
		}
		if(mode==A4988_MODE_HALFSTEP){
			ch0_mode=mode;
			return A4988_OK;
		}
		if(mode==A4988_MODE_QUARTERSTEP){
			ch0_mode=mode;
			return A4988_OK;
		}
		if(mode==A4988_MODE_EIGHTHSTEP){
			ch0_mode=mode;
			return A4988_OK;
		}
		if(mode==A4988_MODE_SIXTEEMTHSTEP){
			ch0_mode=mode;
			return A4988_OK;
		}
	}
	
	if(ch==__A4988_CH1){
		if(mode==A4988_MODE_FULLSTEP){
			ch1_mode=mode;
			return A4988_OK;
		}
		if(mode==A4988_MODE_HALFSTEP){
			ch1_mode=mode;
			return A4988_OK;
		}
		if(mode==A4988_MODE_QUARTERSTEP){
			ch1_mode=mode;
			return A4988_OK;
		}
		if(mode==A4988_MODE_EIGHTHSTEP){
			ch1_mode=mode;
			return A4988_OK;
		}
		if(mode==A4988_MODE_SIXTEEMTHSTEP){
			ch1_mode=mode;
			return A4988_OK;
		}
	}

	
	if(ch==__A4988_CH10){
		if(mode==A4988_MODE_FULLSTEP){
			ch10_mode=mode;
			return A4988_OK;
		}
		if(mode==A4988_MODE_HALFSTEP){
			ch10_mode=mode;
			return A4988_OK;
		}
		if(mode==A4988_MODE_QUARTERSTEP){
			ch10_mode=mode;
			return A4988_OK;
		}
		if(mode==A4988_MODE_EIGHTHSTEP){
			ch10_mode=mode;
			return A4988_OK;
		}
		if(mode==A4988_MODE_SIXTEEMTHSTEP){
			ch10_mode=mode;
			return A4988_OK;
		}
	}
	
	if(ch==__A4988_CH20){
		if(mode==A4988_MODE_FULLSTEP){
			ch20_mode=mode;
			return A4988_OK;
		}
		if(mode==A4988_MODE_HALFSTEP){
			ch20_mode=mode;
			return A4988_OK;
		}
		if(mode==A4988_MODE_QUARTERSTEP){
			ch20_mode=mode;
			return A4988_OK;
		}
		if(mode==A4988_MODE_EIGHTHSTEP){
			ch20_mode=mode;
			return A4988_OK;
		}
		if(mode==A4988_MODE_SIXTEEMTHSTEP){
			ch20_mode=mode;
			return A4988_OK;
		}
	}
	return A4988_ERROR_INVALID_CHANNEL;
}
int a4988chGetMode(int ch){
	if(ch==__A4988_CH0){
			return ch0_mode;
	}	
	if(ch==__A4988_CH1){
			return ch1_mode;
	}	

	if(ch==__A4988_CH10){
			return ch10_mode;
	}	
	
	if(ch==__A4988_CH20){
			return ch20_mode;
	}	
	return A4988_ERROR_INVALID_CHANNEL;
}

int a4988chEnable(int ch){
	if(ch==__A4988_CH0){
		__A4988_CH0_EN_CLR; 
		return A4988_OK;
	}
	if(ch==__A4988_CH1){
		__A4988_CH1_EN_CLR; 
		return A4988_OK;
	}
	if(ch==__A4988_CH10){
		__A4988_CH10_EN_CLR; 
		return A4988_OK;
	}
	if(ch==__A4988_CH20){
		__A4988_CH20_EN_CLR; 
		return A4988_OK;
	}
	return A4988_ERROR_INVALID_CHANNEL;
}
int a4988chDisable(int ch){
	if(ch==__A4988_CH0){
		a4988chStop(ch);
		__A4988_CH0_EN_SET;
		return A4988_OK;
	}
	if(ch==__A4988_CH1){
		a4988chStop(ch);
		__A4988_CH1_EN_SET;
		return A4988_OK;
	}
	if(ch==__A4988_CH10){
		a4988chStop(ch);
		__A4988_CH10_EN_SET;
		return A4988_OK;
	}
	if(ch==__A4988_CH20){
		a4988chStop(ch);
		__A4988_CH20_EN_SET;
		return A4988_OK;
	}
	return A4988_ERROR_INVALID_CHANNEL;
}

int a4988chStart(int ch,int steps){
	int gpv;
	if(ch==__A4988_CH0){
		gpv=a4988chIsBusy(ch,0);
		if(gpv==A4988_BUSY_STAT_BUSY)										return gpv;
		if(steps<0  && gpv==A4988_BUSY_STAT_ERROR_LOW_SENSOR)	   return gpv;
		if(steps>=0 && gpv==A4988_BUSY_STAT_ERROR_HIGH_SENSOR)	return gpv;
		
		if(steps<0){
			steps=-steps;
			ch0_dir=0;
			__A4988_CH0_DIR_CLR;
		}
		else{
			ch0_dir=1;
			__A4988_CH0_DIR_SET;
		}
		
		ch0_stat=0;
		ch0_tick=(steps<<1)+1;
		#ifdef __A4988_CH0_ROTATE_SENSOR
		ch0_rotate_sensor_stat=__A4988_CH0_ROTATE_SENSOR_READ;
		#endif		

		return A4988_OK;
	}
	if(ch==__A4988_CH1){
		gpv=a4988chIsBusy(ch,0);
		if(gpv==A4988_BUSY_STAT_BUSY)										return gpv;
		if(steps<0  && gpv==A4988_BUSY_STAT_ERROR_LOW_SENSOR)	   return gpv;
		if(steps>=0 && gpv==A4988_BUSY_STAT_ERROR_HIGH_SENSOR)	return gpv;
		
		if(steps<0){
			steps=-steps;
			ch1_dir=0;
			__A4988_CH1_DIR_CLR;
		}
		else{
			ch1_dir=1;
			__A4988_CH1_DIR_SET;
		}
		
		ch1_stat=0;
		ch1_tick=(steps<<1)+1;
		#ifdef __A4988_CH1_ROTATE_SENSOR
		ch1_rotate_sensor_stat=__A4988_CH1_ROTATE_SENSOR_READ;
		#endif		

		return A4988_OK;
	}
	
	if(ch==__A4988_CH10){
		gpv=a4988chIsBusy(ch,0);
		if(gpv==A4988_BUSY_STAT_BUSY)										return gpv;
		if(steps<0  && gpv==A4988_BUSY_STAT_ERROR_LOW_SENSOR)	   return gpv;
		if(steps>=0 && gpv==A4988_BUSY_STAT_ERROR_HIGH_SENSOR)	return gpv;
		
		if(steps<0){
			steps=-steps;
			ch10_dir=0;
			__A4988_CH10_DIR_CLR;
		}
		else{
			ch10_dir=1;
			__A4988_CH10_DIR_SET;
		}
		
		ch10_stat=0;
		ch10_tick=(steps<<1)+1;
		#ifdef __A4988_CH10_ROTATE_SENSOR
		ch10_rotate_sensor_stat=__A4988_CH10_ROTATE_SENSOR_READ;
		#endif		

		return A4988_OK;
	}
	
	if(ch==__A4988_CH20){
		gpv=a4988chIsBusy(ch,0);
		if(gpv==A4988_BUSY_STAT_BUSY)										return gpv;
		if(steps<0  && gpv==A4988_BUSY_STAT_ERROR_LOW_SENSOR)	   return gpv;
		if(steps>=0 && gpv==A4988_BUSY_STAT_ERROR_HIGH_SENSOR)	return gpv;
		
		if(steps<0){
			steps=-steps;
			ch20_dir=0;
			__A4988_CH20_DIR_CLR;
		}
		else{
			ch20_dir=1;
			__A4988_CH20_DIR_SET;
		}
		
		ch20_stat=0;
		ch20_tick=(steps<<1)+1;
		#ifdef __A4988_CH20_ROTATE_SENSOR
		ch20_rotate_sensor_stat=__A4988_CH20_ROTATE_SENSOR_READ;
		#endif		

		return A4988_OK;
	}
	return A4988_ERROR_INVALID_CHANNEL	;
}



int a4988chStop(int ch){
	if(ch==__A4988_CH0){
		if(ch0_tick<2)					return  A4988_OK;
		ch0_tick=2;
		return A4988_OK;
	}
	if(ch==__A4988_CH1){
		if(ch1_tick<2)					return  A4988_OK;
		ch1_tick=2;
		return A4988_OK;
	}

	if(ch==__A4988_CH10){
		if(ch10_tick<2)					return  A4988_OK;
		ch10_tick=2;
		return A4988_OK;
	}
	
	if(ch==__A4988_CH20){
		if(ch20_tick<2)					return  A4988_OK;
		ch20_tick=2;
		return A4988_OK;
	}
	return A4988_ERROR_INVALID_CHANNEL;
}

int a4988chIsBusy(int ch,int *ticks){
	if(ch==__A4988_CH0){
		if(ticks)	*ticks=ch0_tick>>1;
		
		if(ch0_stat==0 && ch0_tick) return 1;
		if(ch0_stat<0)					 return ch0_stat;
		return A4988_OK;
	}
	if(ch==__A4988_CH1){
		if(ticks)	*ticks=ch1_tick>>1;
		
		if(ch1_stat==0 && ch1_tick) return 1;
		if(ch1_stat<0)					 return ch1_stat;
		return A4988_OK;
	}
	
	if(ch==__A4988_CH10){
		if(ticks)	*ticks=ch10_tick>>1;
		
		if(ch10_stat==0 && ch10_tick) return 1;
		if(ch10_stat<0)					 return ch10_stat;
		return A4988_OK;
	}
	
	if(ch==__A4988_CH20){
		if(ticks)	*ticks=ch20_tick>>1;
		
		if(ch20_stat==0 && ch20_tick) return 1;
		if(ch20_stat<0)					return ch20_stat;
		return A4988_OK;
	}
	return A4988_ERROR_INVALID_CHANNEL;	
}

int a4988chFeatures(int ch){
	if(ch==__A4988_CH0){	
		return ch0_features;
	}
	if(ch==__A4988_CH1){	
		return ch1_features;
	}
	if(ch==__A4988_CH10){	
		return ch1_features;
	}
	if(ch==__A4988_CH20){	
		return ch20_features;
	}
	return A4988_ERROR_INVALID_CHANNEL;	
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	XY Core AREA	
/*
	this group of functions are designed to manage machine with XY Move Controler
	-----------------------------------------------------------------------------
Warnings:
	1) all hardware init should be done before using this functions
		because hardware should use by many other parts
	2)	all calculation of xy based on Poly Tooth model so
		all functions designed cant work with scrow mode


	update 2022/9/19 #MAHSA_AMINI WE love u
	******************************************************************
	gcodes are passed through all cores and at end cores will select
	relative command to them selves
*/


static int _xy_postion_mode;
static int _xy_x_ch;
static int _xy_y_ch;

static float _xy_x;
static float _xy_y;
static float _xy_rate;

static float _xy_x_step_per_rotate;
static float _xy_x_mode;
static float _xy_x_polly_tooth;
static float _xy_x_tooth_size;

static float _xy_y_step_per_rotate;
static float _xy_y_mode;
static float _xy_y_polly_tooth;
static float _xy_y_tooth_size;



void xyInit(int x_ch,int x_mode,int x_step_per_rotate,int x_polly_tooth,int x_tooth_size
			  ,int y_ch,int y_mode,int y_step_per_rotate,int y_polly_tooth,int y_tooth_size){
	 _xy_x=0;
    _xy_y=0;   
    _xy_rate=1500;

	 _xy_x_ch=x_ch;
	 _xy_y_ch=y_ch;			  
    
    _xy_x_mode=(float)x_mode;
    _xy_x_step_per_rotate=(float)x_step_per_rotate;
    _xy_x_tooth_size=(float)x_tooth_size;
    _xy_x_polly_tooth=(float)x_polly_tooth;

    _xy_y_mode=(float)y_mode;
    _xy_y_step_per_rotate=(float)y_step_per_rotate;
    _xy_y_tooth_size=(float)y_tooth_size;
    _xy_y_polly_tooth=(float)y_polly_tooth;

    _xy_postion_mode=XY_MODE_ABSOLUTE;
}


int xyGc(gCode *cmd,float *outx,float *outy){
	*outx=_xy_x;
	*outy=_xy_y;
	if(cmd->cword=='G' && cmd->ccode==0)	return xyG0(cmd,outx,outy);
	if(cmd->cword=='G' && cmd->ccode==1)	return xyG0(cmd,outx,outy);

	return GCODE_ERROR_UNSUPPORTED_CMD;
}

int xyG0(gCode *cmd,float *outx,float *outy){
    int xstep;
    int ystep;

    float x;
    float y;
        
    float l,t;
    
 	 #if(__XY_G0_G1_MOVE_ALGO==__XY_DIFFRENT_TIME_MODE)
			float tx,ty;
	 #endif
 	 #if(__XY_G0_G1_MOVE_ALGO==__XY_SOFTWARE_STEP_MODE)
	 float m_step;
    float m_val;
    int xcounter,ycounter,m_int,abs_xstep,dir_xstep;
	 #endif
	
    //checking inputs
    *outx=_xy_x;
    *outy=_xy_y;
    if(cmd->key[gArg('X')]==0 && cmd->key[gArg('Y')]==0 && cmd->key[gArg('F')]==0)  return  GCODE_OK; //u have no work to do
    if(cmd->key[gArg('F')])                                                         _xy_rate=cmd->value[gArg('F')];
    if(cmd->key[gArg('X')]==0 && cmd->key[gArg('Y')]==0)                            return  GCODE_OK;  

    x=0;
    if(cmd->key[gArg('X')]){
        if(_xy_postion_mode==XY_MODE_ABSOLUTE)  x=cmd->value[gArg('X')]-_xy_x;
        else                                    x=cmd->value[gArg('X')]+_xy_x;
    }
    y=0;
    if(cmd->key[gArg('Y')]){
        if(_xy_postion_mode==XY_MODE_ABSOLUTE)  y=cmd->value[gArg('Y')]-_xy_y;
        else                                    y=cmd->value[gArg('Y')]+_xy_y;
    }
    
	 
    xstep=(int)((x*_xy_x_step_per_rotate*_xy_x_mode)/(_xy_x_polly_tooth*_xy_x_tooth_size));
    ystep=(int)((y*_xy_y_step_per_rotate*_xy_y_mode)/(_xy_y_polly_tooth*_xy_y_tooth_size));

	 
	 #if(__XY_G0_G1_MOVE_ALGO==__XY_DIFFRENT_TIME_MODE)
	 //arm_sqrt_f32(x*x+y*y,&l);
	 l=sqrt(x*x+y*y);
	 t=((float)1000.0*(float)60.0*l)/(float)(_xy_rate);
    if(xstep!=0){
        tx=t/(float)(2.0*abs(xstep));
		  a4988Tmr0SetDelay((int)(tx*1000));
		  a4988chStart(_xy_x_ch,xstep);
        //apply to motor
    }
    if(ystep!=0){
        ty=t/(float)(2.0*abs(ystep));
        //apply to motor
		 a4988Tmr1SetDelay((int)(ty*1000));
		 a4988chStart(_xy_y_ch,ystep);
    }
	 #endif
	 #if(__XY_G0_G1_MOVE_ALGO==__XY_SOFTWARE_STEP_MODE)
	 l=(_xy_rate*_xy_x_step_per_rotate*_xy_x_mode)/(_xy_x_polly_tooth*_xy_x_tooth_size*(float)60000.0);
	 t=(float)1.0/((float)2.0*l);
	 a4988Tmr0SetDelay((int)(t*1000));
	 a4988Tmr1SetDelay((int)(t*1000));	
	 if(xstep==0)       a4988chStart(_xy_y_ch,ystep);
    else if (ystep==0) a4988chStart(_xy_x_ch,xstep);
    else{
        m_step=((float)ystep/(float)xstep);
        m_val=0;
		  ycounter=0;
        abs_xstep=abs(xstep);
        if(xstep<0)     dir_xstep=-1;
        else            dir_xstep=1;
        for(xcounter=0;xcounter<abs_xstep;xcounter++){
            m_val+=m_step;
            m_int=(int)m_val;
            m_val-=(float)m_int;
            ycounter+=m_int;
            a4988chStart(_xy_x_ch,dir_xstep);
			   while(a4988chIsBusy(_xy_x_ch,0)==A4988_BUSY_STAT_BUSY) osDelay(1);
            if(m_int!=0)	 a4988chStart(_xy_y_ch,m_int);
			   while(a4988chIsBusy(_xy_y_ch,0)==A4988_BUSY_STAT_BUSY) osDelay(1);
        }
        if(ycounter!=ystep){
            a4988chStart(_xy_y_ch,ystep-ycounter);
        }
        
    }
	 #endif
	 #if(__XY_G0_G1_MOVE_ALGO==__XY_NO_MODE)
		l=(_xy_rate*_xy_x_step_per_rotate*_xy_x_mode)/(_xy_x_polly_tooth*_xy_x_tooth_size*(float)60000.0);
		t=(float)1.0/((float)2.0*l);
		a4988Tmr0SetDelay((int)(t*1000));
		a4988Tmr1SetDelay((int)(t*1000));
		a4988chStart(_xy_x_ch,xstep);
		a4988chStart(_xy_y_ch,ystep);
	 #endif
	 
	
	 
    _xy_x+=x;
    _xy_y+=y;
    *outx=_xy_x;
    *outy=_xy_y;
	 while(a4988chIsBusy(_xy_x_ch,0)==A4988_BUSY_STAT_BUSY || a4988chIsBusy(_xy_y_ch,0)==A4988_BUSY_STAT_BUSY) osDelay(1);
    
    return GCODE_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
