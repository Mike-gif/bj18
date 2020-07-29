#ifndef __PTHREAD_CAMERA_H
#define __PTHREAD_CAMERA_H

#include "data_global.h"

extern char dev_camera_mask;
extern int dev_camera_fd;

extern pthread_mutex_t mutex_camera;

extern pthread_cond_t cond_camera;

void *pthread_camera (void *arg);

#endif 