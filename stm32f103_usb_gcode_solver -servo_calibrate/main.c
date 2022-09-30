#include "cmsis_os.h"
#include "rl_usb.h"
#include "GPIO_STM32F10x.h"
#include "a4988.h"
#include "servo.h"


char buff[128];

/*
warnings:
	it cant read zero len array or one char array because according to c string defines it len should be more
	return value : len of string
*/
///////////////////////////////////////////////////////////////////////////////////////////
static int usbd_readline(char *inp,int MaxLen){
	int i,chr;
	MaxLen-=1;
	i=0;
	chr=0;
	do{
	do{ chr=USBD_CDC_ACM_GetChar(0); }while(chr==-1);
	inp[i++]=(char)chr;
	}while(i<MaxLen && chr!='\n' && chr!=0);
	inp[i]=0;
	return i;
}

static int usbd_write(char *inp,int MaxLen){
	int i=0;
	while(i<MaxLen && inp[i]!=0){
		if(USBD_CDC_ACM_PutChar(0,inp[i++])==-1)	return i;
	}
	return i;
}
//////////////////////////////////////////////////////////////////////////////////////////////

int main(){

	int stat;
	gCode inp;
	float x,y;
	osKernelInitialize();
	osKernelStart();
	USBD_Initialize(0);
	USBD_Connect(0);
	GPIO_PinConfigure(GPIOB,12,GPIO_AF_PUSHPULL,GPIO_MODE_OUT50MHZ);
	GPIOB->BSRR=1<<12;
	while(USBD_Configured(0)==false) osDelay(1);
	GPIOB->BRR=1<<12;
	servoGcInit(0);

	a4988Tmr0Init(4000);
	a4988Tmr1Init(4000);
	a4899ChInit(0,A4988_MODE_QUARTERSTEP);
	a4899ChInit(10,A4988_MODE_QUARTERSTEP);
	xyInit(0,A4988_MODE_QUARTERSTEP,200,20,2,10,A4988_MODE_QUARTERSTEP,200,20,2);	

	while(1){
		usbd_readline(buff,128);
		stat=gCodeParse(buff,&inp);
		if(stat==GCODE_OK){
			stat=xyGc(&inp,&x,&y);
			stat=servoGc(&inp,0); 
			
		}
		
		if(stat==GCODE_OK){
			USBD_CDC_ACM_PutChar(0,'O');
			USBD_CDC_ACM_PutChar(0,'K');
			USBD_CDC_ACM_PutChar(0,'\n');
		}
		else{
			USBD_CDC_ACM_PutChar(0,'E');
			USBD_CDC_ACM_PutChar(0,'R');
			USBD_CDC_ACM_PutChar(0,'R');
			USBD_CDC_ACM_PutChar(0,'O');
			USBD_CDC_ACM_PutChar(0,'R');
			USBD_CDC_ACM_PutChar(0,'\n');			
		}
	}
}







