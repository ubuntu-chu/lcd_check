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
#define FIFO_PERM  (O_CREAT|O_EXCL|S_IRUSR | S_IWUSR)
#define FIFO_MODES O_RDONLY
#define FIFO_PATH "/root/project/bin/fifo"
//#define FIFO_PATH "/home/barnard/work/board_9G25/rootfs/root/project/bin/fifo"

enum{
	enum_STATUS_ON = 0,
	enum_STATUS_OFF,
};

ssize_t r_read(int fd, void *buf, size_t size);

int main(void){

	int fd;
    int rval;
	int val;
	char buff[100];
	int pos, status;
	uint16	color;

	if (mkfifo(FIFO_PATH, FIFO_PERM) == -1) {           /* create a named pipe */
		if (errno != EEXIST) {
			fprintf(stderr, "[%ld]:failed to create named pipe %s: %s\n", (long)getpid(), FIFO_PATH, strerror(errno));
			return 1; 
		}
	}
#if 1
	LCD_Open(DEVICE_FILE_PATH);
	GUI_Init();
	GUI_SetBkColor(GUI_WHITE);
	GUI_Clear();
	GUI_DisplayOn();
#endif
#if 0
	GUI_DrawLine(0, 0, 160, 128);
//	GUI_DrawLine(160, 120, 60, 28);
	GUI_DrawLine(10, 100, 10, 28);
	//GUI_DrawLine(0, 10, 160, 10);
	GUI_DrawLine(160, 10, 100, 10);
	//GUI_DrawCircle(50, 50, 10);
	GUI_FillCircle(50, 50, 10);
	GUI_DrawArc(130, 60, 20, 30, 90);
	GUI_DrawEllipse(110, 130, 45, 90);
	GUI_DispStringAt((const uint8 *)"hello st7735 ----jjjjjjjHgggggHHHHH--", 0, 0);
	GUI_DispStringAt((const uint8 *)"world will become better", 0, 100);
	GUI_DispCharAt('A', 100, 60);
	//GUI_DrawRec(0, 0, 40, 20);
	GUI_DrawRect(80, 80, 40, 20);
//	GUI_FillRect(80, 110, 40, 20);
	//GUI_ClearRect(20, 80, 40, 120);
	GUI_DrawBitmap(&bmApplicationIcon, 10, 10);
	GUI_DrawBitmap(&bmApplicationIcon, 120, 50);
	//GUI_DisplayOn();
#endif

	while (((fd = open(FIFO_PATH, FIFO_MODES)) == -1) && (errno == EINTR))  ; 
	if (fd == -1) {
		fprintf(stderr, "[%ld]:failed to open named pipe %s for read: %s\n",
					  (long)getpid(), FIFO_PATH, strerror(errno));    
		return 1; 
	}    
	while (1){
		fprintf(stderr, "[%ld]:about to read...\n", (long)getpid());
		rval = r_read(fd, &val, sizeof(val));
	//	rval = r_read(fd, buff, sizeof(buff));
		if (rval == -1) {
			fprintf(stderr, "[%ld]:failed to read from pipe: %s\n",
					   (long)getpid(), strerror(errno));    
			continue;
		}else if (rval == 0){
			fprintf(stderr, "[%ld]:read len = %d\n", (long)getpid(), rval);
			continue;
		}
		fprintf(stderr, "[%ld]:read len = %d\n", (long)getpid(), rval);
		fprintf(stderr, "[%ld]:read 0x%x\n", (long)getpid(), val);
		//fprintf(stderr, "[%ld]:read %.*s\n", (long)getpid(), rval, buff);
		pos		= (val & 0xffff0000) >> 16;
		status	= val & 0x0000ffff;
		
		if (enum_STATUS_ON == status){
			fprintf(stderr, "[%ld]: pos = %d - status on\n", (long)getpid(), pos);
			color		= GUI_RED;
		}else if (enum_STATUS_OFF == status){
			fprintf(stderr, "[%ld]: pos = %d - status off\n", (long)getpid(), pos);
			color		= GUI_BLACK;
		}else {
			continue;
		}
	#if 1
		GUI_SetColor(color);
		GUI_FillCircle(50, 50, 10);
	#endif
		sleep(1);
	}

	LCD_Close();


	return 0;
}

ssize_t r_read(int fd, void *buf, size_t size) 
{
	ssize_t retval;

	while (1){
		retval = read(fd, buf, size);
		if (retval == -1 && errno == EINTR) {
			continue;
		}else {
			break;
		}
	}
	return retval;
}


