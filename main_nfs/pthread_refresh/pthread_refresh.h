#ifndef __PTHREAD_REFRESH_H
#define __PTHREAD_REFRESH_H

#include "sem.h"
#include "data_global.h"
#include "sqlite_link_list.h"

#define N 1024

extern pthread_mutex_t mutex_refresh;
extern pthread_mutex_t mutex_refresh_updata;
extern pthread_mutex_t mutex_global;
extern pthread_mutex_t mutex_slinklist;
extern pthread_cond_t cond_refresh;
extern pthread_cond_t cond_refresh_updata;
extern pthread_cond_t cond_sqlite;

extern char cgi_status;
extern char qt_status;
extern int shmid; 
extern int semid;
extern struct env_info_clien_addr all_info_RT;

struct shm_addr
{
	char cgi_status;
	char qt_status;
	struct env_info_clien_addr rt_status;
};

void *pthread_refresh (void *arg);

#endif 