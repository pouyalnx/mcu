#ifndef __STEPPER_UNDEPEND_LPC
#define __STEPPER_UNDEPEND_LPC

#define _STEPPER_CH0_STP_PIN_INIT 	LPC_GPIO0->FIODIR|=1
#define _STEPPER_CH0_DIR_PIN_INIT 	LPC_GPIO0->FIODIR|=2
#define _STEPPER_CH0_EN_PIN_INIT 	LPC_GPIO0->FIODIR|=4

#define _STEPPER_CH0_STP_PIN_SET 	LPC_GPIO0->FIOSET=1
#define _STEPPER_CH0_DIR_PIN_SET 	LPC_GPIO0->FIOSET=2
#define _STEPPER_CH0_EN_PIN_SET 		LPC_GPIO0->FIOSET=4

#define _STEPPER_CH0_STP_PIN_CLR 	LPC_GPIO0->FIOCLR=1
#define _STEPPER_CH0_DIR_PIN_CLR 	LPC_GPIO0->FIOCLR=2
#define _STEPPER_CH0_EN_PIN_CLR 		LPC_GPIO0->FIOCLR=4

#define _STEPPER_CH1_STP_PIN_INIT 	LPC_GPIO0->FIODIR|=8
#define _STEPPER_CH1_DIR_PIN_INIT 	LPC_GPIO0->FIODIR|=16
#define _STEPPER_CH1_EN_PIN_INIT 	LPC_GPIO0->FIODIR|=32

#define _STEPPER_CH1_STP_PIN_SET 	LPC_GPIO0->FIOSET=8
#define _STEPPER_CH1_DIR_PIN_SET 	LPC_GPIO0->FIOSET=16
#define _STEPPER_CH1_EN_PIN_SET 		LPC_GPIO0->FIOSET=32

#define _STEPPER_CH1_STP_PIN_CLR 	LPC_GPIO0->FIOCLR=8
#define _STEPPER_CH1_DIR_PIN_CLR 	LPC_GPIO0->FIOCLR=16
#define _STEPPER_CH1_EN_PIN_CLR 		LPC_GPIO0->FIOCLR=32

#define _STEPPER_CH2_STP_PIN_INIT 	LPC_GPIO0->FIODIR|=64
#define _STEPPER_CH2_DIR_PIN_INIT 	LPC_GPIO0->FIODIR|=128
#define _STEPPER_CH2_EN_PIN_INIT 	LPC_GPIO0->FIODIR|=256

#define _STEPPER_CH2_STP_PIN_SET 	LPC_GPIO0->FIOSET=64
#define _STEPPER_CH2_DIR_PIN_SET 	LPC_GPIO0->FIOSET=128
#define _STEPPER_CH2_EN_PIN_SET 		LPC_GPIO0->FIOSET=256

#define _STEPPER_CH2_STP_PIN_CLR 	LPC_GPIO0->FIOCLR=64
#define _STEPPER_CH2_DIR_PIN_CLR 	LPC_GPIO0->FIOCLR=128
#define _STEPPER_CH2_EN_PIN_CLR 		LPC_GPIO0->FIOCLR=256

extern _Bool	stepperUpdate(void);
extern unsigned int stepperMove(int channel,unsigned int steps,int dirction);
extern unsigned int stepperIsBusy(int channel);
extern void stepperConfig(int channel,_Bool hold_en);

#endif
