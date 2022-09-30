#ifndef 	__A4988
#define 	__A4988
#include "gcode.h"
////////////////////////////////////////////////////////////////////////////
/*
	usage docs
	1)	i think something about stm auto timers is wrong because 1ms dont work for steppers
	2) init timer with 4000 to work with all kind of stepper
	3)	why they are better ->write all in one with rich of docs for using
	write all in one file and then based  on application split it to what u want

	4) stepper motor timer spliting for our work
		channel 0-9 		for timer0
		channel 10-19		for timer1
		channel 20-29		for timer2 @we dont implement this one yet

	//init
	a4988TmrInit(4000); //some times 5000 is good
	a4899ChInit(0,A4988_MODE_QUARTERSTEP);



	//simple usage of linear move
	//u can add or remove sensors
	a4988chStart(0,linearMoveToStepDefault(100));
	while(a4988chIsBusy(0,0)==A4988_BUSY_STAT_BUSY) osDelay(1);
	// osDelay(100); for avoid any lack if u go reverse dirctions

	//VIP--> before use select sensors in defins
	//simple usage of rotate sensor with uncomment or comment this sensors
	#define  __A4988_CH0_ROTATE_SENSOR
	
	//angle mode
	a4988chStart(0,angleRotateToStepDefault(90));
	while(a4988chIsBusy(0,0)==A4988_BUSY_STAT_BUSY) osDelay(1);
	osDelay(100);

	//xyCore machine
	a4988TmrInit(5000);
	a4899ChInit(0,A4988_MODE_QUARTERSTEP);
	xyCoreInitDefault(0,1);

*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	i have new idea if all of ur codes and change only be in one lib
	how our coding it will so better and advance help time save
	improve this file only for stepper and dont write new one

	a4988 or other kind of drivers that based on three
	wire enable,step,dir
	
	nots: in this driver for motor controling we use
	equal square pulse Low period and High period is
	equal

*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////	stepper motor linear move based on float motor	/////////////////////////////////////

#define 	MOTOR_STEP_PER_REVOLUTION_200		200
#define 	MOTOR_STEP_PER_REVOLUTION_400		400
#define 	MOTOR_STEP_PER_REVOLUTION_48		48
#define	BELT_PITCH_IN_MM_2_GT2_DEFAULT	2
#define	POLLY_TOOTH_COUNT_20_DEFAULT			20
#define	POLLY_TOOTH_COUNT_16			16



extern int linearMoveToStep(double mm,int a4988_mode,int motor_step_per_revolution,int belt_pich_mm,int poly_tooth_count);
extern int linearMoveToStepDefault(double mm);
extern double stepToLinearMove(int step,int a4988_mode,int motor_step_per_revolution,int belt_pich_mm,int poly_tooth_count);
extern double stepToLinearMoveDefault(int step);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////	stepper motor Rotary model based on angel			/////////////////////////////////////

extern int angleRotateToStep(double deg,int a4988_mode,int motor_step_per_revolution);
extern int angleRotateToStepDefault(double deg);


extern double stepToAngleRotate(int step,int a4988_mode,int motor_step_per_revolution);
extern double stepToangleRotateDefault(int step);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////		 A4988 controller timing						/////////////////////////////////////
/*
	for all channels only one timer enough u can use this timer for diffrent purpose but all move controls 
	will be managed by this timer simpler resource manage and operation controlling
	
stepper motor timer spliting for our work
		channel 0-9 		for timer0
		channel 10-19		for timer1
		channel 20-29		for timer2 @we dont implement this one yet
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define __A4988_TMR0_CLK_ENABLE					RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,RCC_APB1ENR_TIM2EN)
#define __A4988_TMR0									TIM2
#define __A4988_TMR0_IRQn							TIM2_IRQn
#define __A4988_TMR0_IRQHandler					TIM2_IRQHandler
#define __A4988_TMR0_FREQUENCY					1000000
#define __A4988_TMR0_INPUT_FREQ					PCLK1_Frequency

extern int a4988Tmr0Init(int delay_us);
extern void a4988Tmr0DeInit(void);
extern int a4988Tmr0GetDelay(void);
extern int a4988Tmr0SetDelay(int delay_us);

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define __A4988_TMR1_CLK_ENABLE					RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,RCC_APB1ENR_TIM3EN)
#define __A4988_TMR1									TIM3
#define __A4988_TMR1_IRQn							TIM3_IRQn
#define __A4988_TMR1_IRQHandler					TIM3_IRQHandler
#define __A4988_TMR1_FREQUENCY					1000000
#define __A4988_TMR1_INPUT_FREQ					PCLK1_Frequency


extern int a4988Tmr1Init(int delay_us);
extern void a4988Tmr1DeInit(void);
extern int a4988Tmr1GetDelay(void);
extern int a4988Tmr1SetDelay(int delay_us);
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define __A4988_TMR2_CLK_ENABLE					RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,RCC_APB1ENR_TIM4EN)
#define __A4988_TMR2									TIM4
#define __A4988_TMR2_IRQn							TIM4_IRQn
#define __A4988_TMR2_IRQHandler					TIM5_IRQHandler
#define __A4988_TMR2_FREQUENCY					1000000
#define __A4988_TMR2_INPUT_FREQ					PCLK1_Frequency


extern int a4988Tmr2Init(int delay_us);
extern void a4988Tmr2DeInit(void);
extern int a4988Tmr2GetDelay(void);
extern int a4988Tmr2SetDelay(int delay_us);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////		 A4988 options										/////////////////////////////////////
/*
Microstep Resolution
Low	Low	Low	Full step
High	Low	Low	Half step
Low	High	Low	Quarter step
High	High	Low	Eighth step
High	High	High	Sixteenth step

*/
#define 	A4988_MODE_FULLSTEP				1
#define 	A4988_MODE_HALFSTEP				2	
#define 	A4988_MODE_QUARTERSTEP			4
#define 	A4988_MODE_EIGHTHSTEP				8
#define 	A4988_MODE_SIXTEEMTHSTEP			16

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////		 A4988 channels defines							/////////////////////////////////////
/*
	SENSORS 
	supported sensors are digital sensors
	we have two kind sensor
	
	**edge and accident sensors
				 							__
			___							|_|												    ___
sensor low	|						^^^MMM^^^												|sensor High
				|						VVVMMMVVV												|
				----------------------------------------------------------------------
	when motor rich to end of each plane sensor should stop motor from more move and
	physical demage to motor or mechnical part errors and ...

																		 ____________________________________
	sensor output should be like ______________________|

	**angle start sensors
	this kind used for hook rotate zero positions
	
				
		 _________________
		|						|			 _________
		|						|			|			 |  ______________
		|						|			|	hook	_| |
		O		stepper		O========X		  |_<=| rotate sensor
		|		Motor			|			|			 | |______________
		|						|			|_________|
		|_________________|
		
	
	
	this kind of sensors dont need to stop motor stop for ever and returning error in all stats
	we just need to have pulse signal
									
																_______________________
	sensort output should be like ______________|							  |______________________

   according to this explans we use two diffrent kind sensor
	all supported sensors is three enabled by typedef



	LOW SENSOR refer to dir pin low stat move dirction
	HIG SENSOR refer to dir pin High stat and it move dirction
	ROT SENSOR refer to rotate sensor that trigred
	
	
	__LM Prename equal to Linear Move
	
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define	__A4988_CH0							0

#define	__A4988_CH0_EN_CLK_ENABLE				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,RCC_APB2ENR_IOPBEN)
#define	__A4988_CH0_EN_PORT						GPIOB
#define	__A4988_CH0_EN_PIN						GPIO_Pin_6						
#define	__A4988_CH0_EN_SET						GPIOB->BSRR=__A4988_CH0_EN_PIN
#define	__A4988_CH0_EN_CLR						GPIOB->BRR=__A4988_CH0_EN_PIN

#define	__A4988_CH0_DIR_CLK_ENABLE				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,RCC_APB2ENR_IOPBEN)
#define	__A4988_CH0_DIR_PORT						GPIOB
#define	__A4988_CH0_DIR_PIN						GPIO_Pin_5						
#define	__A4988_CH0_DIR_SET						GPIOB->BSRR=__A4988_CH0_DIR_PIN
#define	__A4988_CH0_DIR_CLR						GPIOB->BRR=__A4988_CH0_DIR_PIN

#define	__A4988_CH0_STP_CLK_ENABLE				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,RCC_APB2ENR_IOPBEN)
#define	__A4988_CH0_STP_PORT						GPIOB
#define	__A4988_CH0_STP_PIN						GPIO_Pin_7					
#define	__A4988_CH0_STP_SET						GPIOB->BSRR=__A4988_CH0_STP_PIN
#define	__A4988_CH0_STP_CLR						GPIOB->BRR=__A4988_CH0_STP_PIN



//#define  __A4988_CH0_LM_HIGH_SENSOR
#define	__A4988_CH0_LM_HIGH_SENSOR_CLK_ENABLE		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,RCC_APB2ENR_IOPBEN)
#define	__A4988_CH0_LM_HIGH_SENSOR_PORT				GPIOB
#define	__A4988_CH0_LM_HIGH_SENSOR_PIN				GPIO_Pin_4					
#define	__A4988_CH0_LM_HIGH_SENSOR_TRIGRED			__A4988_CH0_LM_HIGH_SENSOR_PORT->IDR&__A4988_CH0_LM_HIGH_SENSOR_PIN



//#define  __A4988_CH0_LM_LOW_SENSOR
#define	__A4988_CH0_LM_LOW_SENSOR_CLK_ENABLE		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,RCC_APB2ENR_IOPBEN)
#define	__A4988_CH0_LM_LOW_SENSOR_PORT				GPIOB
#define	__A4988_CH0_LM_LOW_SENSOR_PIN				   GPIO_Pin_1					
#define	__A4988_CH0_LM_LOW_SENSOR_TRIGRED			__A4988_CH0_LM_LOW_SENSOR_PORT->IDR&__A4988_CH0_LM_LOW_SENSOR_PIN	


// for future usage it need to improved because it depend on zero or high level check
//#define  __A4988_CH0_ROTATE_SENSOR
#define	__A4988_CH0_ROTATE_SENSOR_CLK_ENABLE		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,RCC_APB2ENR_IOPBEN)
#define	__A4988_CH0_ROTATE_SENSOR_PORT				GPIOB
#define	__A4988_CH0_ROTATE_SENSOR_PIN				   GPIO_Pin_0					
#define	__A4988_CH0_ROTATE_SENSOR_READ			  	__A4988_CH0_ROTATE_SENSOR_PORT->IDR&__A4988_CH0_ROTATE_SENSOR_PIN	

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define	__A4988_CH1									1

#define	__A4988_CH1_EN_CLK_ENABLE				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,RCC_APB2ENR_IOPBEN)
#define	__A4988_CH1_EN_PORT						GPIOB
#define	__A4988_CH1_EN_PIN						GPIO_Pin_11						
#define	__A4988_CH1_EN_SET						GPIOB->BSRR=__A4988_CH1_EN_PIN
#define	__A4988_CH1_EN_CLR						GPIOB->BRR=__A4988_CH1_EN_PIN

#define	__A4988_CH1_DIR_CLK_ENABLE				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,RCC_APB2ENR_IOPBEN)
#define	__A4988_CH1_DIR_PORT						GPIOB
#define	__A4988_CH1_DIR_PIN						GPIO_Pin_12					
#define	__A4988_CH1_DIR_SET						GPIOB->BSRR=__A4988_CH1_DIR_PIN
#define	__A4988_CH1_DIR_CLR						GPIOB->BRR=__A4988_CH1_DIR_PIN

#define	__A4988_CH1_STP_CLK_ENABLE				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,RCC_APB2ENR_IOPBEN)
#define	__A4988_CH1_STP_PORT						GPIOB
#define	__A4988_CH1_STP_PIN						GPIO_Pin_13					
#define	__A4988_CH1_STP_SET						GPIOB->BSRR=__A4988_CH1_STP_PIN
#define	__A4988_CH1_STP_CLR						GPIOB->BRR=__A4988_CH1_STP_PIN



//#define  __A4988_CH1_LM_HIGH_SENSOR
#define	__A4988_CH1_LM_HIGH_SENSOR_CLK_ENABLE		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,RCC_APB2ENR_IOPBEN)
#define	__A4988_CH1_LM_HIGH_SENSOR_PORT				GPIOB
#define	__A4988_CH1_LM_HIGH_SENSOR_PIN				GPIO_Pin_14					
#define	__A4988_CH1_LM_HIGH_SENSOR_TRIGRED			__A4988_CH1_LM_HIGH_SENSOR_PORT->IDR&__A4988_CH1_LM_HIGH_SENSOR_PIN



//#define  __A4988_CH1_LM_LOW_SENSOR
#define	__A4988_CH1_LM_LOW_SENSOR_CLK_ENABLE		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,RCC_APB2ENR_IOPBEN)
#define	__A4988_CH1_LM_LOW_SENSOR_PORT				GPIOB
#define	__A4988_CH1_LM_LOW_SENSOR_PIN				   GPIO_Pin_15					
#define	__A4988_CH1_LM_LOW_SENSOR_TRIGRED			__A4988_CH1_LM_LOW_SENSOR_PORT->IDR&__A4988_CH1_LM_LOW_SENSOR_PIN	


// for future usage it need to improved because it depend on zero or high level check
//#define  __A4988_CH1_ROTATE_SENSOR
#define	__A4988_CH1_ROTATE_SENSOR_CLK_ENABLE		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,RCC_APB2ENR_IOPBEN)
#define	__A4988_CH1_ROTATE_SENSOR_PORT				GPIOB
#define	__A4988_CH1_ROTATE_SENSOR_PIN				   GPIO_Pin_10					
#define	__A4988_CH1_ROTATE_SENSOR_READ			  	__A4988_CH1_ROTATE_SENSOR_PORT->IDR&__A4988_CH1_ROTATE_SENSOR_PIN	

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define	__A4988_CH10									10

#define	__A4988_CH10_EN_CLK_ENABLE					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,RCC_APB2ENR_IOPAEN)
#define	__A4988_CH10_EN_PORT							GPIOA
#define	__A4988_CH10_EN_PIN							GPIO_Pin_5						
#define	__A4988_CH10_EN_SET							GPIOA->BSRR=__A4988_CH10_EN_PIN
#define	__A4988_CH10_EN_CLR							GPIOA->BRR=__A4988_CH10_EN_PIN

#define	__A4988_CH10_DIR_CLK_ENABLE				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,RCC_APB2ENR_IOPAEN)
#define	__A4988_CH10_DIR_PORT						GPIOA
#define	__A4988_CH10_DIR_PIN							GPIO_Pin_6					
#define	__A4988_CH10_DIR_SET							GPIOA->BSRR=__A4988_CH10_DIR_PIN
#define	__A4988_CH10_DIR_CLR							GPIOA->BRR=__A4988_CH10_DIR_PIN

#define	__A4988_CH10_STP_CLK_ENABLE				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,RCC_APB2ENR_IOPAEN)
#define	__A4988_CH10_STP_PORT						GPIOA
#define	__A4988_CH10_STP_PIN							GPIO_Pin_7					
#define	__A4988_CH10_STP_SET							GPIOA->BSRR=__A4988_CH10_STP_PIN
#define	__A4988_CH10_STP_CLR							GPIOA->BRR=__A4988_CH10_STP_PIN



//#define  __A4988_CH10_LM_HIGH_SENSOR

#define	__A4988_CH10_LM_HIGH_SENSOR_CLK_ENABLE		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,RCC_APB2ENR_IOPAEN)
#define	__A4988_CH10_LM_HIGH_SENSOR_PORT				GPIOA
#define	__A4988_CH10_LM_HIGH_SENSOR_PIN				GPIO_Pin_8				
#define	__A4988_CH10_LM_HIGH_SENSOR_TRIGRED			__A4988_CH10_LM_HIGH_SENSOR_PORT->IDR&__A4988_CH10_LM_HIGH_SENSOR_PIN



//#define  __A4988_CH10_LM_LOW_SENSOR
#define	__A4988_CH10_LM_LOW_SENSOR_CLK_ENABLE		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,RCC_APB2ENR_IOPAEN)
#define	__A4988_CH10_LM_LOW_SENSOR_PORT				GPIOA
#define	__A4988_CH10_LM_LOW_SENSOR_PIN				   GPIO_Pin_9					
#define	__A4988_CH10_LM_LOW_SENSOR_TRIGRED			__A4988_CH10_LM_LOW_SENSOR_PORT->IDR&__A4988_CH10_LM_LOW_SENSOR_PIN	


// for future usage it need to improved because it depend on zero or high level check
//#define  __A4988_CH10_ROTATE_SENSOR
#define	__A4988_CH10_ROTATE_SENSOR_CLK_ENABLE		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,RCC_APB2ENR_IOPAEN)
#define	__A4988_CH10_ROTATE_SENSOR_PORT				GPIOA
#define	__A4988_CH10_ROTATE_SENSOR_PIN				   GPIO_Pin_10					
#define	__A4988_CH10_ROTATE_SENSOR_READ			  	__A4988_CH10_ROTATE_SENSOR_PORT->IDR&__A4988_CH10_ROTATE_SENSOR_PIN	

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define	__A4988_CH20								20

#define	__A4988_CH20_EN_CLK_ENABLE				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,RCC_APB2ENR_IOPAEN)
#define	__A4988_CH20_EN_PORT						GPIOA
#define	__A4988_CH20_EN_PIN						GPIO_Pin_0						
#define	__A4988_CH20_EN_SET						GPIOA->BSRR=__A4988_CH20_EN_PIN
#define	__A4988_CH20_EN_CLR						GPIOA->BRR=__A4988_CH20_EN_PIN

#define	__A4988_CH20_DIR_CLK_ENABLE			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,RCC_APB2ENR_IOPAEN)
#define	__A4988_CH20_DIR_PORT					GPIOA
#define	__A4988_CH20_DIR_PIN						GPIO_Pin_1						
#define	__A4988_CH20_DIR_SET						GPIOA->BSRR=__A4988_CH20_DIR_PIN
#define	__A4988_CH20_DIR_CLR						GPIOA->BRR=__A4988_CH20_DIR_PIN

#define	__A4988_CH20_STP_CLK_ENABLE			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,RCC_APB2ENR_IOPAEN)
#define	__A4988_CH20_STP_PORT					GPIOA
#define	__A4988_CH20_STP_PIN						GPIO_Pin_2					
#define	__A4988_CH20_STP_SET						GPIOA->BSRR=__A4988_CH20_STP_PIN
#define	__A4988_CH20_STP_CLR						GPIOA->BRR=__A4988_CH20_STP_PIN



//#define  __A4988_CH20_LM_HIGH_SENSOR
#define	__A4988_CH20_LM_HIGH_SENSOR_CLK_ENABLE		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,RCC_APB2ENR_IOPAEN)
#define	__A4988_CH20_LM_HIGH_SENSOR_PORT				GPIOA
#define	__A4988_CH20_LM_HIGH_SENSOR_PIN				GPIO_Pin_3					
#define	__A4988_CH20_LM_HIGH_SENSOR_TRIGRED			__A4988_CH20_LM_HIGH_SENSOR_PORT->IDR&__A4988_CH20_LM_HIGH_SENSOR_PIN



//#define  __A4988_CH20_LM_LOW_SENSOR
#define	__A4988_CH20_LM_LOW_SENSOR_CLK_ENABLE		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,RCC_APB2ENR_IOPAEN)
#define	__A4988_CH20_LM_LOW_SENSOR_PORT				GPIOA
#define	__A4988_CH20_LM_LOW_SENSOR_PIN				GPIO_Pin_4					
#define	__A4988_CH20_LM_LOW_SENSOR_TRIGRED			__A4988_CH20_LM_LOW_SENSOR_PORT->IDR&__A4988_CH20_LM_LOW_SENSOR_PIN	


// for future usage it need to improved because it depend on zero or high level check
//#define  __A4988_CH20_ROTATE_SENSOR
#define	__A4988_CH20_ROTATE_SENSOR_CLK_ENABLE		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,RCC_APB2ENR_IOPBEN)
#define	__A4988_CH20_ROTATE_SENSOR_PORT				GPIOA
#define	__A4988_CH20_ROTATE_SENSOR_PIN				GPIO_Pin_4					
#define	__A4988_CH20_ROTATE_SENSOR_READ			  	__A4988_CH20_ROTATE_SENSOR_PORT->IDR&__A4988_CH20_ROTATE_SENSOR_PIN	

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////		 A4988 stepper motor function	outputs   	/////////////////////////////////////


#define A4988_OK	   					         0
#define A4988_ERROR_INVALID_CHANNEL	   		-100

#define A4988_BUSY_STAT_FREE						0
#define A4988_BUSY_STAT_BUSY				      1
#define A4988_BUSY_STAT_ERROR_HIGH_SENSOR	  -1
#define A4988_BUSY_STAT_ERROR_LOW_SENSOR	  -2
#define A4988_BUSY_STAT_ERROR_ROT_SENSOR	  -3

#define	A4988_FEATURE_LOW_SENSOR				1
#define	A4988_FEATURE_HIGH_SENSOR				2
#define	A4988_FEATURE_ROTATE_SENSOR			4


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////		 A4988 stepper motor controlling function	/////////////////////////////////////
/*
	functions manual and notes

+----------------------------------------------------------------------------------------------------------+
	int a4988chIsBusy(int ch,int *leftSteps);
	
	leftSteps if equal to zero or null nothing will be return but have address it
	will be return tickmemory>>1
	
	
	return Value>0 motor is moving
	return value=0 motor is free
	return value<0	errors-->
						-1		sensor A trigred
						-2		sensor B trigred
*/


extern int a4899ChInit(int ch,int mode);
extern int a4988chSetMode(int ch,int mode);
extern int a4988chGetMode(int ch);
extern int a4988chEnable(int ch);
extern int a4988chDisable(int ch);
extern int a4988chStart(int ch,int steps);
extern int a4988chStop(int ch);
extern int a4988chIsBusy(int ch,int *ticks);
extern int a4988chFeatures(int ch);


//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
///	XY move machine	based on positions it can go any dirctions that u want
//////////////////////////////////////////////////////////////////////////////////////////////
/*
	this app very week against end axis becuase we dont apply it sensor
	in this app we use Glevel commands attention
	
	xyCoreIsGOOD name for this part but for execute good model we will need more advance system
	that need two diffrent timer hardware and it hard to implement by system according to many 
	sounds and software control also have simpler system but hard to implement
	
	we what more update in this driver that make it more better

*/

#define __XY_DIFFRENT_TIME_MODE		2
#define __XY_SOFTWARE_STEP_MODE		1
#define __XY_NO_MODE						0
#define __XY_G0_G1_MOVE_ALGO			__XY_NO_MODE

#define XY_MODE_ABSOLUTE    0
#define XY_MODE_RELATIVE    1



extern void xyInit(int x_ch,int x_mode,int x_step_per_rotate,int x_polly_tooth,int x_tooth_size
						,int y_ch,int y_mode,int y_step_per_rotate,int y_polly_tooth,int y_tooth_size);
extern int xyGc(gCode *cmd,float *outx,float *outy);


//direct access but only for test
extern int xyG0(gCode *cmd,float *outx,float *outy); 



#endif

















