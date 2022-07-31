#include "app.h"
#include <cmsis_os.h>
#include <stdio.h>
#include "gcode.h"
#include "gcmd.h"

extern const char gcode_buffer[];


void app(void *arg){
	(void)arg;
	gMachineInit();
	osDelay(5000);
	while(1){
		printf("Lnx Embroidrey 2022.\n");
		gcodeRun(gcode_buffer,1);
			osDelay(50000);
	}
}

