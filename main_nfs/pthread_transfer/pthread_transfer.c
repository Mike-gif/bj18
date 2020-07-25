#include "pthread_transfer.h"

#define LEN_ENV 20
#define LEN_RFID 4 

extern int dev_uart_fd;

extern linklist linkHead;

extern pthread_cond_t cond_analysis;

extern pthread_mutex_t mutex_linklist;

void serial_init(int fd)
{
    struct termios options;
    tcgetattr(fd, &options);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~CSIZE;
    options.c_cflag &= ~CRTSCTS;
    options.c_cflag |= CS8;
    options.c_cflag &= ~CSTOPB;
    options.c_iflag |= IGNPAR;
    options.c_iflag &= ~(ICRNL | IXON);
    options.c_oflag = 0;
    options.c_lflag = 0;

    cfsetispeed(&options, B115200);
    cfsetispeed(&options, B115200);
    tcsetattr(fd, TCSANOW, &options);

}

void *pthread_transfer (void *arg)
{
    int i, len;
    char flag = 0, check;
	link_datatype buf;

   linkHead = CreateEmptyLinklist();
#if 1
   if((dev_uart_fd = open(DEV_ZIGBEE, O_RDWR)) < 0)
   {
      perror("open ttyUSB");
      return -1;
   }
   serial_init(dev_uart_fd);

   printf("pthread_transfer is ok\n");
#endif   
   while(1)
   {
       memset(&buf, 0, sizeof(link_datatype));
       read(dev_uart_fd, &check, 1);
       if(check == 'c')  //胡磊2020.07.21：未理解'c' 's' 'r'是怎么来的
       {
           sendMsgQueue(MSG_M0, MSG_CONNECT_SUCCESS);
       }
       if(check == 's')
       {
           check = 0;
           read(dev_uart_fd, &check, 1);
           if(check == 't')
           {
             check = 0;
             read(dev_uart_fd, &check, 1);
             if(check == ':')
             {
               check = 0;
               read(dev_uart_fd, &check, 1);
               if(check == 'e')
               {
                   buf.msg_type = 'e';
                   usleep(1);
                   if((len = read(dev_uart_fd, buf.text, LEN_ENV)) != LEN_ENV) //胡磊2020.07.21：将串口的值读入buf.text
                   {
                       for(i = len; i < LEN_ENV; i++)
                       {
                           read(dev_uart_fd, buf.text + i, 1);
                       }
                       flag = 1;
                   }
               }
                   else if(check == 'r')
                   {
                       buf.msg_type = 'r';
                       usleep(1);
                       if((len = read(dev_uart_fd, buf.text, LEN_RFID)) != LEN_RFID)
                       {
                           for(i = len; i < LEN_ENV; i++)
                           {
                               read(dev_uart_fd, buf.text+i, 1);
                           }
                       }   
                           flag = 1;
                    }
             } 
           }
        }

           if(1 == flag)
           {
               pthread_mutex_lock(&mutex_linklist);
               if((InsertLinknode(buf)) == -1)
               {
                   pthread_mutex_unlock(&mutex_linklist);
                   printf("NONMEM\n");
               }
               pthread_mutex_unlock(&mutex_linklist);
               flag = 0;
               pthread_cond_signal(&cond_analysis);
           }
       
   } 
   return 0;
}
