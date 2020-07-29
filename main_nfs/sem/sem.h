#ifndef __SEM_H
#define __SEM_H

#include "data_global.h"

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct  seminfo *__buf;
     
};

#endif 