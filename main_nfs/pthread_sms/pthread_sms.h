#ifndef __PTHREAD_SMS_H
#define __PTHREAD_SMS_H

#include "data_global.h"

struct pdu_info {
    char cnswap[32];
    char phswap[32];
};

extern void gprs_serial_init(int fd);
extern int send_message(int dev_fd,int flag);
extern void *pthread_sms (void *);

extern pthread_mutex_t mutex_sms;
extern pthread_cond_t cond_sms;

extern char recive_phone[12];
extern char center_phone[12];
extern unsigned char dev_sms_cmd;

extern int dev_sms_fd;


#endif