#include "pthread_camera.h"

void *pthread_camera (void *arg)
{
   unsigned char picture = 0;

   if((dev_camera_fd = open(DEV_CAMERA, O_RDWR | O_NOCTTY | O_NDELAY)) < 0 )
   {
       printf("Can't open file /tmp/webcom\n");
       exit(-1);
   }
   printf("pthread_camera is ok\n");
   while(1)
   {
       pthread_mutex_lock(&mutex_camera);
       pthread_cond_wait(&cond_camera, &mutex_camera);
       picture = dev_camera_mask;
       printf("dev_camera_mask = %d\n", dev_camera_mask);
       pthread_mutex_unlock(&mutex_camera);

       switch (picture)
       {
       case 1:
           write(dev_camera_fd, "one", 3);
           break;
       case 3:
           write(dev_camera_fd, "three", 5);
           break;
       case 5:
           write(dev_camera_fd, "five", 4);
           break;
       case 7:
           write(dev_camera_fd, "seven", 5);
           break;
       case 9:
           write(dev_camera_fd, "nine", 9);
           break;
       default:
           break;
       }
   }
}