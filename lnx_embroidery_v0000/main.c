#include <rl_usb.h>
#include <cmsis_os.h>
#include "app.h"



osThreadDef(app,osPriorityNormal,1,0);
int main(){
		osKernelInitialize();
		osKernelStart();
		USBD_Initialize(0);
		USBD_Connect(0);
		while(USBD_Configured(0)==false);  //Warning: it dont excute app until usb configure
		osThreadCreate(osThread(app),NULL);
		while(1) osDelay(osWaitForever);
}

