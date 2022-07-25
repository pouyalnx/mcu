#ifndef _esp_rs_h
#define _esp_rs_h






extern volatile int esp_stat;
#define ESP_STAT_OK_READY 0
#define ESP_STAT_ERROR    1
#define ESP_STAT_BUSY     2
#define ESP_STAT_BUSY_TX  3

typedef struct{
	unsigned char stat;
}users;

extern users user[6];

#define USER_STAT_OFFLINE 0
#define USER_STAT_ONLINE  1 






extern void esp_init(void);




#endif

