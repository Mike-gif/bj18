#ifndef __PTHREAD_BUZZER_H
#define __PTHREAD_BUZZER_H

#include "data_global.h"

extern int dev_buzzer_fd;
extern char dev_buzzer_mask;

extern pthread_mutex_t mutex_buzzer;

extern pthread_cond_t cond_buzzer;

void *pthread_buzzer(void *arg);

#endif