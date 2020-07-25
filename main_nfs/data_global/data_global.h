#ifndef __DATA_GLOBAL_H
#define __DATA_GLOBAL_H

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <termios.h>
#include <syscall.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include "uart_cache.h"
#include <stdint.h>
#include <unistd.h>
#include <linux/input.h>

#define DEV_ZIGBEE 			"/dev/ttyUSB1"
#define SQLITE_OPEN			"/warehouse.db"

#define STORAGE_NUM 		5

#define GOODS_NUM			10

#define SMS_TEM			1
#define SMS_HUM			2	
#define SMS_ILL			3	
#define SMS_BRE			4
#define SMS_STRNO1		0
#define SMS_STRNO2		1<<7

#define QUEUE_MSG_LEN		32

#define MSG_LED				1L  
#define MSG_LED_TEM_ON		0x22
#define MSG_LED_TEM_OFF		0x20
#define MSG_LED_HUM_ON		0x22
#define MSG_LED_HUM_OFF		0x20
#define MSG_LED_ILL_ON		0x11
#define MSG_LED_ILL_OFF		0x10
#define MSG_LED_TRI_ON		0x11
#define MSG_LED_TRI_OFF		0x10

#define MSG_BEEP			2L
#define MSG_BEEP_OFF		0
#define MSG_BEEP_ON			1

#define MSG_CAMERA			3L

#define MSG_M0				4L
#define MSG_CONNECT_SUCCESS 'Y'
#define MSG_M0_FAN_OFF		0x00
#define MSG_M0_FAN_ON1		0x01
#define MSG_M0_FAN_ON2		0x02
#define MSG_M0_FAN_ON3		0x03

#define COLLECT_INSERTER	0x10

#define MSG_M0_BEEP_OFF		0x10
#define MSG_M0_BEEP_ON		0x11

#define MSG_M0_LED_OFF		0x20
#define MSG_M0_LED_ON		0x21

#define ENV_UPDATE			0x00
#define GOODS_ADD			0x20
#define GOODS_REDUCE		0x21
#define GOODS_GET			0x22

extern void *pthread_sqlite (void *);			//数据库线程
extern void *pthread_analysis (void *);			//数据解析线程
extern void *pthread_transfer (void *);			//数据接收线程

extern void *pthread_client_request (void *);	//接收CGI，QT请求

struct storage_goods_info
{
	unsigned char goods_type;
	unsigned int goods_count;
};

struct storage_info
{
	unsigned char storage_status;				// 0:open 1:close
	unsigned char led_status;
	unsigned char buzzer_status;
	unsigned char fan_status;
	unsigned char seg_status;
	signed char x;
	signed char y;
	signed char z;
	char samplingTime[20];
	float temperature;
	float temperatureMIN;
	float temperatureMAX;
	float humidity;
	float humidityMIN;
	float humidityMAX;
	float illumination;
	float illuminationMIN;
	float illuminationMAX;
	float battery;
	float adc;
	float adcMIN;
	struct storage_goods_info goods_info[GOODS_NUM];
};

struct env_info_clien_addr
{
	struct storage_info storage_no[STORAGE_NUM];	
};
struct sqlite_operation
{
	int table_select_mask;
	int env_operation_mask;
	int table_operation_mask;
	int goods_operation_mask;
};

struct msg 
{
    long type;
	long msgtype;
	unsigned char text[QUEUE_MSG_LEN];
};

void sendMsgQueue(long type, unsigned char text);




#endif