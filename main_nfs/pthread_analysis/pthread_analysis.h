#ifndef __PTHREAD_ANALYSIS_H
#define __PTHREAD_ANALYSIS_H
#include "data_global.h"
#include "link_list.h"
#include "sqlite_link_list.h"

float dota_atof (char unitl);
int dota_atoi (const char * cDecade);
float dota_adc (unsigned int ratio);
int storageAllgood (int sto_no);
int checkEnv (int sto_no, struct storage_info *p);
void getEnvPackage (link_datatype *buf);
void getGoodsPackage (link_datatype *buf);
void *pthread_analysis (void *arg);
#endif 