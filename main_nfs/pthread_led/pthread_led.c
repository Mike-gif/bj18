#include "pthread_led.h"

void *pthread_led (void *arg)
{
    int led_no;
    char *buff = NULL;
    unsigned char led_set;
    if((dev_led_fd = open("/sys/class/leds/led2/brightness", O_RDWR)) < 0)
    {
        printf("Can't open file /sys/class/leds/led2/brightness\n");
        exit(-1);
    }
    printf("pthread_led is ok\n");
    while(1)
    {
      pthread_mutex_lock(&mutex_led);
      pthread_cond_wait(&cond_led, &mutex_led);
      led_set = dev_led_mask;
      pthread_mutex_unlock(&mutex_led);
      printf("pthread_led is wake up\n");
      printf("led_set = %#x\n", led_set);
      if(led_set == 0x11)
      {
          if((dev_led_fd = open("/sys/class/leds/led2/brightness", O_RDWR)) < 0)
          {
              printf("Can't open file /sys/class/leds/led2/brightness\n");
              exit(-1);
          }
          buff = "1";
          write(dev_led_fd, buff, 1);
          printf("fs4412 led2 on\n");
          close(dev_led_fd);
      }
      if (led_set == 0x22)
		{
			if ((dev_led_fd = open ("/sys/class/leds/led3/brightness", O_RDWR)) < 0)
			{
				printf ("Cann't open file /sys/class/leds/led3/brightness\n");
				exit (-1);
			}
			buff = "1";
			write(dev_led_fd, buff, 1);
			printf("fs4412 led3 on\n");
			close(dev_led_fd);
		}
		if (led_set == 0x20)
		{
			if ((dev_led_fd = open ("/sys/class/leds/led3/brightness", O_RDWR)) < 0)
			{
				printf ("Cann't open file /sys/class/leds/led3/brightness\n");
				exit (-1);
			}
			buff = "0";
			write(dev_led_fd, buff, 1);
			printf("fs4412 led3 off\n");
			close(dev_led_fd);
		}
    }
}