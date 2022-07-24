#include "pen.h"
#include "cmsis_os.h"



void app(void *arg){
	(void)arg;
	penInit();
	while(1){
		penMoveUp();
		while(penGetStat() & 2)
		osDelay(5000);
		penMoveDown();
		while(penGetStat() & 2)
		osDelay(5000);
	}
	
}
osThreadDef(app,osPriorityNormal,1,0);


int main(){
	osKernelInitialize();
	osThreadCreate(osThread(app),NULL);
	osKernelStart();
	while(1) osDelay(osWaitForever);
}
