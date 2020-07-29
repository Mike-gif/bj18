#include "main.h"



void setLimit (int sto_no, float temMAX, float temMIN, float humMAX, float humMIN, float illMAX, float illMIN)
{
	if (sto_no >= 0 && (sto_no <=STORAGE_NUM - 1))
	{
		all_info_RT.storage_no[sto_no].temperatureMAX = temMAX;
		all_info_RT.storage_no[sto_no].temperatureMIN = temMIN;
		all_info_RT.storage_no[sto_no].humidityMAX = humMAX;
		all_info_RT.storage_no[sto_no].humidityMIN = humMIN;
		all_info_RT.storage_no[sto_no].illuminationMAX = illMAX;
		all_info_RT.storage_no[sto_no].illuminationMIN = illMIN;
	}
}


void ReleaseResource (int signo)
{
	pthread_mutex_destroy (&mutex_linklist);
	pthread_mutex_destroy (&mutex_global);
    pthread_mutex_destroy (&mutex_sqlite);
   	pthread_mutex_destroy (&mutex_analysis);
	pthread_mutex_destroy (&mutex_client_request);
	pthread_mutex_destroy (&mutex_sms);
	pthread_mutex_destroy (&mutex_camera);
	pthread_mutex_destroy (&mutex_led);
	pthread_mutex_destroy (&mutex_buzzer);
	pthread_mutex_destroy (&mutex_refresh_updata);
	pthread_mutex_destroy (&mutex_refresh);

    
	pthread_cond_destroy (&cond_refresh_updata);
	pthread_cond_destroy (&cond_refresh);
    pthread_cond_destroy (&cond_sqlite);
	pthread_cond_destroy (&cond_analysis);
	pthread_cond_destroy (&cond_client_request);
	pthread_cond_destroy (&cond_sms);
	pthread_cond_destroy (&cond_camera);
	pthread_cond_destroy (&cond_led);
	pthread_cond_destroy (&cond_buzzer);

    pthread_cancel (id_refresh);
	pthread_cancel (id_transfer);
    pthread_cancel (id_sqlite);
    pthread_cancel (id_analysis);
	pthread_cancel (id_client_request);
	pthread_cancel (id_sms);
	pthread_cancel (id_camera);
	pthread_cancel (id_led);
	pthread_cancel (id_buzzer);


	close (dev_camera_fd);
	close (dev_led_fd);
	close (dev_buzzer_fd);
	close (dev_sms_fd);
	close (dev_uart_fd);
    printf ("All quit\n");

	exit(0);
}

int main(int argc, char **argv)
{
#if 1
	Create_table ();  //创建环境表
#endif
   	setLimit (1, 50, 5, 50, 5, 4000, 15);
	setLimit (2, 50, 5, 50, 5, 4000, 15);

    pthread_mutex_init (&mutex_sqlite, NULL);
	pthread_mutex_init (&mutex_analysis, NULL);
	pthread_mutex_init (&mutex_linklist, NULL);
	pthread_mutex_init (&mutex_client_request, NULL);
	pthread_mutex_init (&mutex_buzzer, NULL);
	pthread_mutex_init (&mutex_led, NULL);
	pthread_mutex_init (&mutex_camera, NULL);
	pthread_mutex_init (&mutex_global, NULL);
	pthread_mutex_init (&mutex_sms, NULL);
	pthread_mutex_init (&mutex_refresh, NULL);
	pthread_mutex_init (&mutex_refresh_updata, NULL);
	

    pthread_cond_init (&cond_sqlite, NULL);
	pthread_cond_init (&cond_analysis, NULL);
	pthread_cond_init (&cond_client_request, NULL);
	pthread_cond_init (&cond_buzzer, NULL);
	pthread_cond_init (&cond_led, NULL);
	pthread_cond_init (&cond_camera, NULL);
	pthread_cond_init (&cond_sms, NULL);
	pthread_cond_init (&cond_refresh, NULL);
	pthread_cond_init (&cond_refresh_updata, NULL);
	

    signal (SIGINT, ReleaseResource);

    pthread_create (&id_sqlite, 0, pthread_sqlite, NULL);
	sleep (1);
	pthread_create (&id_analysis, 0, pthread_analysis, NULL);
    pthread_create (&id_transfer, 0, pthread_transfer, NULL);
	sleep (1);
	pthread_create (&id_client_request, 0, pthread_client_request, NULL);
//	pthread_create (&id_infrared, 0, pthread_infrared, NULL);
	pthread_create (&id_buzzer, 0, pthread_buzzer, NULL);
	pthread_create (&id_led, 0, pthread_led, NULL);
	pthread_create (&id_camera, 0, pthread_camera, NULL);
	pthread_create (&id_sms, 0, pthread_sms, NULL);
	pthread_create (&id_refresh, 0, pthread_refresh, NULL);
	

    pthread_join (id_sqlite, NULL);
	printf ("g1\n");
	pthread_join (id_analysis, NULL);
	printf ("g2\n");
	pthread_join (id_transfer, NULL);
	printf ("g3\n");
	
	pthread_join (id_client_request, NULL);
	printf ("g5\n");
//	pthread_join (id_infrared, NULL);
	printf ("g6\n");
	pthread_join (id_buzzer, NULL);
	printf ("g7\n");
	pthread_join (id_led, NULL);
	printf ("g8\n");
	pthread_join (id_camera, NULL);
	printf ("g9\n");
	pthread_join (id_sms, NULL);
	printf ("g10\n");
	pthread_join (id_refresh, NULL);
	printf ("g11\n");
    return 0;
}
