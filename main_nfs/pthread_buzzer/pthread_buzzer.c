#include "pthread_buzzer.h"


void *pthread_buzzer(void *arg)
{
    struct input_event event;
    if((dev_buzzer_fd = open(DEV_BUZZER, O_RDWR | O_NONBLOCK)) < 0)
    {
        printf("Can't open file /dev/beep\n");
        exit(-1);
    }
    printf("pthread_buzzer is ok\n");

    while(1)
    {
        pthread_mutex_lock(&mutex_buzzer);
        pthread_cond_wait(&cond_buzzer, &mutex_buzzer);
        printf("pthread_buzzer is wake up\n");
        if(dev_buzzer_mask){
            event.type = EV_SND;
            event.code = SND_TONE;
            event.value = 100;
            write(dev_buzzer_fd, &event, sizeof(struct input_event));
            printf("**buzzer on\n");
        }else{
            event.type = EV_SND;
            event.code = SND_TONE;
            event.value = 0;
            write(dev_buzzer_fd, &event, sizeof(struct input_event));
            printf("**buzzer off\n");
        }
        pthread_mutex_unlock(&mutex_buzzer);
    }
    return 0;
}