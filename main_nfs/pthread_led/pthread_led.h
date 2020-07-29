#ifndef __PTHREAD_LED_H
#define __PTHREAD_LED_H

#include "data_global.h"

extern unsigned char dev_led_mask;
extern int dev_led_fd;

extern pthread_mutex_t mutex_led;

extern pthread_cond_t cond_led;

void *pthread_led (void *);				//LED控制线程

#endif 