#include "esp_1.h"
#include <stdio.h>
#include "Driver_USART.h"
#include "cmsis_os2.h"
#include <stdio.h>


extern ARM_DRIVER_USART Driver_USART6;
ARM_DRIVER_USART *drv;


char esp_buffer[ESP_RESIVER_BUFFER_SIZE];
int checked_data_loc;
int checked_cmd_loc;
int command_stat;



char *send_data;
int send_data_size;


char *res_data;
int res_data_size;

/*********************************************************************
/application:
/input:
/output:
/note:
*********************************************************************/

void drv_callback(uint32_t event){
	if(event & ARM_USART_EVENT_RECEIVE_COMPLETE){
		drv->Receive(esp_buffer,ESP_RESIVER_BUFFER_SIZE);
     checked_data_loc=0;
	}
}



void esp_io_init(void){
	drv=&Driver_USART6;
	drv->Initialize(drv_callback);
	drv->PowerControl(ARM_POWER_FULL);
	drv->Control(ARM_USART_MODE_ASYNCHRONOUS,115200);
	drv->Control(ARM_USART_CONTROL_RX,1);
	drv->Control(ARM_USART_CONTROL_TX,1);
	drv->Receive(esp_buffer,ESP_RESIVER_BUFFER_SIZE);	
}

/*********************************************************************
/application:
/input:
/output:
/note:
*********************************************************************/
int esp_cmd(char *command,int c_size,int time_out){
	int gpv0,gpv1;
	checked_cmd_loc=drv->GetRxCount();
	if(drv->Send(command,c_size)!=ARM_DRIVER_OK) return 0;
	gpv0=0;
	command_stat=esp_cmd_timeout;		
			while(gpv0++<time_out){
				osDelay(1);
				//puts(esp_buffer);
				gpv1=drv->GetRxCount();
				for(;checked_cmd_loc<gpv1;checked_cmd_loc++){
					if(esp_buffer[checked_cmd_loc]=='K' && checked_cmd_loc>0 && esp_buffer[checked_cmd_loc-1]=='O'){
						command_stat=esp_cmd_ok;
						gpv0=time_out;
						break;
					}
					if(esp_buffer[checked_cmd_loc]=='R' && checked_cmd_loc>3 && esp_buffer[checked_cmd_loc-1]=='R'&& esp_buffer[checked_cmd_loc-2]=='O'&& esp_buffer[checked_cmd_loc-3]=='R'){
					  command_stat=esp_cmd_error;
						gpv0=time_out;
						break;
					}
					if(esp_buffer[checked_cmd_loc]=='L' && checked_cmd_loc>2 && esp_buffer[checked_cmd_loc-1]=='I'&& esp_buffer[checked_cmd_loc-2]=='A'&& esp_buffer[checked_cmd_loc-3]=='F'){
					  command_stat=esp_cmd_error;
						gpv0=time_out;
						break;
					}
				}
			}
	return command_stat;
}

int esp_send(char *command,int c_size,char *data,int data_s){
	int gpv0,gpv1,time_out;
	checked_cmd_loc=drv->GetRxCount();
	if(drv->Send(command,c_size)!=ARM_DRIVER_OK) return -2;		
	send_data=data;
	send_data_size=data_s;
	time_out=2000;  //default
  gpv0=0;
	command_stat=esp_cmd_timeout;	
			while(gpv0++<time_out){
				osDelay(1);
				gpv1=drv->GetRxCount();
				for(;checked_cmd_loc<gpv1;checked_cmd_loc++){
					if(esp_buffer[checked_cmd_loc]=='>'){
						command_stat=esp_cmd_ok;
						///send data
						
			drv->Send(send_data,send_data_size);			
			while(gpv0++<time_out){
				osDelay(1);
				gpv1=drv->GetRxCount();
				for(;checked_cmd_loc<gpv1;checked_cmd_loc++){
					if(esp_buffer[checked_cmd_loc]=='K' && checked_cmd_loc>0 && esp_buffer[checked_cmd_loc-1]=='O'){
						command_stat=esp_cmd_ok;
						gpv0=time_out;
						break;
					}
					if(esp_buffer[checked_cmd_loc]=='R' && checked_cmd_loc>3 && esp_buffer[checked_cmd_loc-1]=='R'&& esp_buffer[checked_cmd_loc-2]=='O'&& esp_buffer[checked_cmd_loc-3]=='R'){
					  command_stat=esp_cmd_error;
						gpv0=time_out;
						break;
					}
				}
			}						
						break;
					}
					if(esp_buffer[checked_cmd_loc]=='R' && checked_cmd_loc>3 && esp_buffer[checked_cmd_loc-1]=='R'&& esp_buffer[checked_cmd_loc-2]=='O'&& esp_buffer[checked_cmd_loc-3]=='R'){
					  command_stat=esp_cmd_error;
						gpv0=time_out;
						break;
					}
				}
			}
	return command_stat;
}


int esp_stat(void){
return command_stat;
}

/*********************************************************************
/application:
/input:
/output:
/note:
*********************************************************************/
int find_sign(int *gpv0){
	while(*gpv0>0){
		if(esp_buffer[*gpv0]==',') return 1;
	  (*gpv0)--;
	}
	return 0;
}


char * esp_download(int *size){
int gpv0,gpv1,gpv2;
gpv0=checked_data_loc;
gpv1=drv->GetRxCount();	

	while(gpv0<gpv1){
	if(esp_buffer[gpv0]==':'){
	gpv2=gpv0;
	if(find_sign(&gpv2) && sscanf(&esp_buffer[gpv2+1],"%d",&res_data_size)==1){
	gpv0++;
	if(res_data_size<=(gpv1-gpv0))	{
	res_data=&esp_buffer[gpv0];
	  checked_data_loc=gpv0+res_data_size;
		*size=res_data_size;
		return res_data;
	}
	else{
		return NULL;
	}
	}
	else{
	checked_data_loc=gpv0+1;  //for avoid reprocess
	}		
	}

	gpv0++;
}
  checked_data_loc=gpv0;
	return NULL;
}




	