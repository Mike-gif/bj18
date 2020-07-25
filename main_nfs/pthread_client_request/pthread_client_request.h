#ifndef PTHREAD_CLIENT_REQUEST
#define PTHREAD_CLIENT_REQUEST

#include "data_global.h"
#include "uart_cache.h"
#include "sqlite_link_list.h"
#include <stdio.h>

extern unsigned char dev_led_mask;
extern unsigned char dev_camera_mask;
extern unsigned char dev_buzzer_mask;
extern unsigned char dev_uart_mask;

extern pthread_cond_t cond_led;
extern pthread_cond_t cond_camera;
extern pthread_cond_t cond_buzzer;
extern pthread_cond_t cond_refresh;
extern pthread_cond_t cond_uart_cmd;
extern pthread_cond_t cond_sqlite;

extern pthread_mutex_t mutex_global;
extern pthread_mutex_t mutex_uart_cmd;
extern pthread_mutex_t mutex_led;
extern pthread_mutex_t mutex_buzzer;
extern pthread_mutex_t mutex_camera;
extern pthread_mutex_t mutex_slinklist;

extern int msgid;
extern uart_cache_list m0_cache_head, m0_cache_tail;
extern char recive_phone[12] ;
extern char center_phone[12] ;
extern struct env_info_clien_addr all_info_RT;
struct setEnv
{
	int temMAX;
	int temMIN;
	int humMAX;
	int humMIN;
	int illMAX;
	int illMIN;
};

void *pthread_client_request (void *arg);
#endif 