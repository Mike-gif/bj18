#include "pthread_sms.h"

//获取线程号
pid_t gettid(void)
{
    return syscall(SYS_gettid);
}

//A8与GPRS模块之间的初始化串口
void gprs_serial_init(int fd)
{
    struct termios options;

    tcgetattr(fd, &options);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~CSIZE;
    options.c_cflag &= ~CRTSCTS;
    options.c_cflag |= CS8;
    options.c_cflag &= ~CSTOPB;
    options.c_iflag |= IGNPAR;

    options.c_oflag = 0;
    options.c_lflag = 0;

    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);
    tcsetattr(fd, TCSANOW, &options);
    printf("init gprs over...\n");
    return 0;
}

void swap(char number[], char swap[])
{
    char ch1[] = "86";
    char tmp[16];
    int i;

    memset(swap, 0, 32);
    memset(tmp, 0, 16);
    strcpy(swap, number);
    strcat(swap, "f");
    strcat(ch1, swap);
    strcpy(swap, ch1);

    for(i = 0; i <= strlen(swap) - 1; i += 2)
    {
        tmp[i + 1] = swap[i];
        tmp[i] = swap[i + 1];
    }
    strcpy(swap, tmp);
}

int send(int fd,char *cmgf,char *cmgs,char *message)//发送函数，用于AT指令的发送
{
  printf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&fd : %d", fd);
	int nread,nwrite;
	char buff[128];
	char reply[128];

    memset(buff, 0, sizeof(buff));
    strcpy(buff, "AT\r");
    nwrite = write(fd, buff, strlen(buff));
    printf("1_nwrite = %d, %s\n", nwrite, buff);

    memset(reply, 0, sizeof(reply));
    sleep(1);
    nread = read(fd, reply, sizeof(reply));
    printf("2_nread = %d, %s\n", nread, reply);

    memset(buff, 0, sizeof(buff));
    strcpy(buff, "AT+CMGF=");
    strcat(buff, cmgf);
    strcat(buff, "\r");
    nwrite = write(fd, buff, strlen(buff));
    printf("3_nwrite = %d, %s\n", nwrite, buff);

    memset(reply, 0, sizeof(reply));
    sleep(1);
    nread = read(fd, reply, sizeof(reply));
    printf("4_nread = %d, %s\n", nread, reply);

    memset(buff, 0, sizeof(buff));
    strcpy(buff, "AT+CMGS=");
    strcat(buff, cmgs);
    strcat(buff, "\r");
    nwrite = write(fd, buff, strlen(buff));
    printf("5_nwrite = %d, %s\n", nwrite, buff);

    memset(reply,0,sizeof(reply));
	sleep(1);
	nread = read(fd,reply,sizeof(reply));
	printf("6_nread = %d, %s\n", nread, reply);

    memset(buff,0,sizeof(buff));
	strcpy(buff,message);
	nwrite = write(fd,buff,strlen(buff));
	printf("7_nwrite = %d, %s\n", nwrite, buff);

    memset(reply,0,sizeof(reply));
	sleep(1);
	nread = read(fd,reply,sizeof(reply));
	printf("8_nread = %d, %s\n", nread, reply);
	printf("call send over\n");	
		return 0; 
}

int send_message(int dev_fd, int flag)
{
    printf("&&&&&&&&&&&&&&&&&&&&& dev_fd: %d\n", dev_fd);
	gprs_serial_init(dev_fd);	//串口初始化
	char cmgf[] = "0";
    char cmgs[4] = {'\0'};
    char ch2[] = "0891";
    char ch3[] = "1100";
    char ch4[] = "000800";
    char ch5[] = "0d91";
    char final[128];
    char *message[8] = {
    //    "0A6E295EA65F025E38FF01",//仓库着火啦！
	"104ED35E934E006E295EA68D856807FF01",   // 温度超标
	"104ED35E934E0067098D3C95EF5165FF01",
	"104ED35E934E00514971678D856807FF01",
	"104ED35E934E006E7F5EA68D856807FF01",
	"104ED35E934E8C6E295EA68D856807FF01",
	"104ED35E934E8C67098D3C95EF5165FF01",
	"104ED35E934E8C514971678D856807FF01",
	"104ED35E934E8C6E7F5EA68D856807FF01",
      //  "0A4ED35E9367094EBAFF01",//仓库有贼闯入！
      //  "0A514971675F025E38FF01",//光照超标！
      //  "0A6E7F5EA65F025E38FF01"//湿度超标！
        };
    struct pdu_info pdu;
    int len;
    memset(final, 0, 80);
    printf("******swap*******\n");
    swap(recive_phone, pdu.phswap);
    swap(center_phone, pdu.cnswap);
    printf("recive_phone = %s, center_phone = %s\n", recive_phone, center_phone);
    printf("******strcpy*****\n");
    strcpy(final, ch2);
    strcat(final, pdu.cnswap);
    strcat(final, ch3);
    strcat(final, ch5);
    strcat(final, pdu.phswap);
    strcat(final, ch4);
    printf("*****strcat*****\n");
    strcat(final, message[flag]);
    strcat(final, "\x1a");
    printf("****len****\n");
    len = strlen(ch3)+ strlen(ch4) +strlen(ch5) + strlen(pdu.phswap)+strlen(message[flag]);
    puts(final);
    sprintf(cmgs, "%d", len/2);
    puts(final);
    printf("****send****\n");
    send(dev_fd, cmgf, cmgs, final);

    return 0;
}

//向A8主线程中获得GPRS的线程
void *pthread_sms(void *arg)
{
  unsigned char buf;
  if((dev_sms_fd = open(DEV_GPRS, O_RDWR | O_NOCTTY | O_NDELAY)) < 0)
  {
      printf("Fail to open DEV GSM!\n");
      return -1;
  }
  printf("pthread_sms is ok\n");

  while(1)
  {
      pthread_mutex_lock(&mutex_sms);
      pthread_cond_wait(&cond_sms, &mutex_sms);
      buf = dev_sms_cmd;
      printf("pthread_sms is wake up! dev_sms_cmd = %d\n", buf);
      pthread_mutex_unlock(&mutex_sms);
      switch(buf)
      {
        case SMS_STRNO1|SMS_TEM:
            send_message(dev_sms_fd, SMS_MSG_TEM1);
        case SMS_STRNO1 | SMS_BRE:
			send_message(dev_sms_fd, SMS_MSG_BRE1);
			break;
		case SMS_STRNO1 | SMS_ILL:
			send_message(dev_sms_fd, SMS_MSG_ILL1);
			break;
		case SMS_STRNO1 | SMS_HUM:
			send_message(dev_sms_fd, SMS_MSG_HUM1);
			break;
		case SMS_STRNO2 | SMS_TEM:
			send_message(dev_sms_fd, SMS_MSG_TEM2);
			break;
		case SMS_STRNO2 | SMS_BRE:
			send_message(dev_sms_fd, SMS_MSG_BRE2);
			break;
		case SMS_STRNO2 | SMS_ILL:
			send_message(dev_sms_fd, SMS_MSG_ILL2);
			break;
		case SMS_STRNO2 | SMS_HUM:
			send_message(dev_sms_fd, SMS_MSG_HUM2);
			break;
      }
  }
     return 0;
}