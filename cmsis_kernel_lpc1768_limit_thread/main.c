#include "cmsis_os.h"
#include "LPC17xx.h"



void led(unsigned int arg[]){
	unsigned int bit=1<<arg[0];
	unsigned int delay=arg[1];
	LPC_GPIO0->FIODIR|=bit;
	
	while(1){
		LPC_GPIO0->FIOSET|=bit;
		osDelay(delay);
		LPC_GPIO0->FIOCLR|=bit;
		osDelay(delay);
	}	
}
osThreadDef(led,osPriorityNormal,12,0);




int main(){
	unsigned int data[2];
	osKernelInitialize();
	data[0]=0;
	data[1]=100;
	osThreadCreate(osThread(led),data);
	osKernelStart();
}



