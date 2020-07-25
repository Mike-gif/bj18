#ifndef __PTHREAD_TRANSFER
#define __PTHREAD_TRANSFER

#include "link_list.h"
#include "data_global.h"

void *pthread_transfer (void *arg);
void serial_init(int fd);

#endif 