#include "lcd_check.h"
#include <stdio.h>      /*标准输入输出定义*/
#include <stdlib.h>
#include <unistd.h>     /*Unix标准函数定义*/
#include <sys/types.h>  /**/
#include <sys/stat.h>   /**/
#include <fcntl.h>      /*文件控制定义*/
#include <termios.h>    /*PPSIX终端控制定义*/
#include <errno.h>      /*错误号定义*/
#include <getopt.h>
#include <string.h>
#include <time.h>


#define DEVICE_FILE_PATH	"/dev/st7735"


int main(void){

	int fd, fd2;

	fd = open(DEVICE_FILE_PATH, O_RDONLY);
	if (fd < 0){
		perror("open failed\n");
	}
	fd2 = open(DEVICE_FILE_PATH, O_RDONLY);
	if (fd2 < 0){
		perror("open failed\n");
	}

	sleep(10);
	while (1);

	close(fd);

	printf("hello st7735\n");

	return 0;
}



