#ifndef __MAIN_H
#define __MAIN_H

#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include "data_global.h"
#include "pthread_sqlite.h"
#include "pthread_analysis.h"
#include "pthread_transfer.h"
#include "pthread_client_request.h"

extern pthread_cond_t cond_sqlite;
extern pthread_cond_t cond_analysis;
extern pthread_cond_t cond_client_request;
extern pthread_cond_t cond_buzzer;
extern pthread_cond_t cond_led;
extern pthread_cond_t cond_camera;
extern pthread_cond_t cond_sms;
extern pthread_cond_t cond_refresh;
extern pthread_cond_t cond_refresh_updata;

extern pthread_mutex_t mutex_sqlite;
extern pthread_mutex_t mutex_analysis;
extern pthread_mutex_t mutex_linklist;
extern pthread_mutex_t mutex_client_request;
extern pthread_mutex_t mutex_buzzer;
extern pthread_mutex_t mutex_led;
extern pthread_mutex_t mutex_camera;
extern pthread_mutex_t mutex_sms;
extern pthread_mutex_t mutex_global;
extern pthread_mutex_t mutex_refresh;
extern pthread_mutex_t mutex_refresh_updata;

extern int dev_buzzer_fd;
extern int dev_led_fd;
extern int dev_camera_fd;
extern int dev_sms_fd;
extern int dev_uart_fd;



extern struct env_info_clien_addr all_info_RT; 

pthread_t 	id_sqlite,
            id_analysis,
            id_transfer,
            id_client_request,
            id_buzzer,
			id_led,
			id_camera,
			id_sms,
            id_refresh;


//extern void Create_table ();
void setLimit (int sto_no, float temMAX, float temMIN, float humMAX, float humMIN, float illMAX, float illMIN);
void ReleaseResource (int signo);

#endif 