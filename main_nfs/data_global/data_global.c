#include "data_global.h"
#include "uart_cache.h"
#include "sqlite_link_list.h"

uart_cache_list m0_cache_head, m0_cache_tail;

char recive_phone[12] = "0";
char center_phone[12] = "0";



int msgid;
int dev_uart_fd;
int dev_camera_fd;
int dev_sms_fd;
int dev_buzzer_fd;
int dev_led_fd;

/**** sqlite module ****/
int storageNum;
int goodsKinds;

struct env_info_clien_addr env_info_clien_addr_s;
struct env_info_clien_addr all_info_RT;

unsigned char dev_sms_cmd; 
unsigned char dev_led_mask;
unsigned char dev_buzzer_mask;
unsigned char dev_camera_mask;
unsigned char dev_uart_mask;


slinklist slinkHead, slinkTail;

pthread_cond_t cond_sqlite;
pthread_cond_t cond_analysis;
pthread_cond_t cond_uart_cmd;
pthread_cond_t cond_refresh;
pthread_cond_t cond_sms;
pthread_cond_t cond_client_request;
pthread_cond_t cond_led;
pthread_cond_t cond_buzzer;
pthread_cond_t cond_camera;
pthread_cond_t cond_sms;
pthread_cond_t cond_refresh;
pthread_cond_t cond_refresh_updata;

pthread_mutex_t mutex_uart_cmd;
pthread_mutex_t mutex_global;
pthread_mutex_t mutex_sqlite;
pthread_mutex_t mutex_linklist;
pthread_mutex_t mutex_slinklist;
pthread_mutex_t mutex_analysis;
pthread_mutex_t mutex_sms;
pthread_mutex_t mutex_client_request;
pthread_mutex_t mutex_led;
pthread_mutex_t mutex_buzzer;
pthread_mutex_t mutex_camera;
pthread_mutex_t mutex_refresh;
pthread_mutex_t mutex_refresh_updata;
pthread_mutex_t mutex_infrared;


void sendMsgQueue(long type, unsigned char text)
{
    struct msg msgbuf;
    msgbuf.type = 1L;
    msgbuf.msgtype = type;
    msgbuf.text[0] = text;
    msgsnd(msgid, &msgbuf, sizeof(msgbuf) - sizeof(long), 0);
}