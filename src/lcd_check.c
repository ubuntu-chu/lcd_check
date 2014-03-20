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

	LCD_Open(DEVICE_FILE_PATH);
	GUI_Init();
	GUI_DisplayOn();
	GUI_SetBkColor(GUI_YELLOW);
	GUI_Clear();
	GUI_DrawLine(0, 0, 160, 128);
//	GUI_DrawLine(160, 120, 60, 28);
	GUI_DrawLine(10, 100, 10, 28);
	//GUI_DrawLine(0, 10, 160, 10);
	GUI_DrawLine(160, 10, 100, 10);
	//GUI_DrawCircle(50, 50, 10);
	GUI_FillCircle(50, 50, 10);
	GUI_DrawArc(130, 60, 20, 30, 90);
	GUI_DrawEllipse(110, 130, 45, 90);
	GUI_DispStringAt("hello st7735 ----jjjjjjjHgggggHHHHH--", 0, 0);
	//GUI_DispStringAt("world will become better", 0, 80);
	GUI_DispCharAt('A', 100, 60);
	//GUI_DrawRec(0, 0, 40, 20);
	GUI_DrawRect(80, 80, 40, 20);
//	GUI_FillRect(80, 110, 40, 20);
	//GUI_ClearRect(20, 80, 40, 120);
	GUI_DrawBitmap(&bmApplicationIcon, 10, 10);
	GUI_DrawBitmap(&bmApplicationIcon, 120, 50);
	//GUI_DisplayOn();

	while (1);

	LCD_Close();


	return 0;
}



