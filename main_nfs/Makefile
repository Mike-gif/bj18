CROSS_COMPILE  ?= arm-linux-gnueabihf-
#CFLAGS         ?=-Wall -c -g
LDFLAGS        ?= -lpthread -L ./lib -l sqlite3
TARGET         ?= storage_manage

CC            := $(CROSS_COMPILE)gcc 

#LIBPATH       := -lgcc -L /usr/local/arm/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/lib/gcc/arm-linux-gnueabihf/4.9.4
#include用于存放各种系统文件
INCUDIRS		:=  include\
				data_global\
				pthread_sqlite\
				sqlite_link_list\
				pthread_transfer\
				pthread_analysis\
				pthread_client_request\
				uart_cache\
				link_list

SRCDIRS			:= project\
				data_global\
				pthread_sqlite\
				sqlite_link_list\
				pthread_transfer\
				pthread_analysis\
				pthread_client_request\
				uart_cache\
				link_list


INCLUDE         := $(patsubst %, -I %, $(INCUDIRS))

#获取SRCDIRS所对应各目录下的.c文件
CFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))
#去掉各个.c文件的目录，获得他们的名字
CFILENDIR		:= $(notdir $(CFILES))

#将编译出的.o文件放在obj目录下
OBJS			:= $(patsubst %, obj/%, $(CFILENDIR:.c=.o)) 
#OBJS			:= $(patsubst %.c, %.o, $(CFILES))
VPATH           := $(SRCDIRS)
#CFLAGS = -Wall -nostdlib -fno-builtin -O2 $(INCLUDE) 

.PHONY:clean
$(TARGET) : $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
$(OBJS) : obj/%.o : %.c
	$(CC) -Wall -nostdlib -c -O2 $(INCLUDE) -o $@ $< 
#	$(CC) -Wall -c $(INCLUDE) -o $@ $<
#	$(CC) $(CFLAGS) -o $@ $<
#	$(CC) -Wall -c $(INCLUDE) -o $@ $<
#	$(CC) $(CFLAGS)  $(INCLUDE) -o $@ $<
#	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o storage_manage  #warehouse.db
print:
	@echo INCLUDE = $(INCLUDE)
	@echo CFILES = $(CFILES)
	@echo CFILENDIR = $(CFILENDIR)
	@echo OBJS = $(OBJS)