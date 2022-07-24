#include <LPC17xx.h>
#include <cmsis_os.h>


void ledapp(void *args)
{
	LPC_GPIO0->FIODIR|=1;
	while(1){
		LPC_GPIO0->FIOSET|=1;
		osDelay(1000);
		LPC_GPIO0->FIOCLR|=1;
		osDelay(1000);
	}
}
osThreadDef(ledapp,osPriorityNormal,3,0);

///////////////////////////////////////////////////////////////////////////////
int main(){
	osThreadId thread_id;
	osKernelInitialize();
	thread_id=osThreadCreate(osThread(ledapp),NULL);
	osKernelStart();
	return 0;
}
