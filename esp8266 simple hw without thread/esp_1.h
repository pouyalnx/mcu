#ifndef _esp_1
#define _esp_1



#define ESP_RESIVER_BUFFER_SIZE 2048
extern char esp_buffer[ESP_RESIVER_BUFFER_SIZE];

enum command_stat{
	esp_cmd_ok,
	esp_cmd_error,
	esp_cmd_timeout
};



///////////////////////////////////////////////////////////////////////////////////////////////////
extern void  esp_io_init(void);
extern int   esp_cmd(char *command,int c_size,int time_out_v);
extern int   esp_send(char *command,int c_size,char *data,int data_s);
extern char *esp_download(int *size);
extern int   esp_stat(void);

#endif
