#include "gui.h"
#include <stdio.h>      /*标准输入输出定义*/
#include <stdlib.h>
#include <unistd.h>     /*Unix标准函数定义*/
#include <sys/types.h>  /**/
#include <sys/stat.h>   /**/
#include <sys/ioctl.h>   /**/
#include <fcntl.h>      /*文件控制定义*/
#include <errno.h>      /*错误号定义*/

#define  DEBUG		(0)

static GUI_COLOR		s_BkColor;
static GUI_COLOR		s_Color;
static int				s_LcdFd = -1;
static uint32		    s_X		= 0;
static uint32		    s_Y		= 0;

static struct lcd_info   t_lcdInfo;

static uint16  GUI_Color2Index_565(uint32 Color);

int LCD_Open(const char *path)
{
	s_LcdFd = open(path, O_RDONLY);
	if (s_LcdFd < 0){
		perror("lcd device open failed\n");
	}

	return s_LcdFd;
}

int LCD_Ioctl(int fd, unsigned int request, void *arg)
{
	int ret;

	if (fd < 0){
		printf("lcd device ioctl fd < 0\n");
	}
	ret = ioctl(fd , request, arg);
	if (ret != 0){
		perror("lcd device ioctl failed\n");
	}

	return ret;
}


void LCD_Close(void)
{
	if (s_LcdFd > 0){
		close(s_LcdFd);
	}
}

void coordinate_check(struct coordinate *pcoordinate)
{
	if (pcoordinate->m_x >= t_lcdInfo.m_width){
		pcoordinate->m_x= t_lcdInfo.m_width - 1;
	}
	if (pcoordinate->m_y >= t_lcdInfo.m_height){
		pcoordinate->m_y= t_lcdInfo.m_height - 1;
	}
}

void coordinate_pair_check(struct coordinate_pair *pcoordinate_pair)
{
	if (pcoordinate_pair->m_x1 >= t_lcdInfo.m_width){
		pcoordinate_pair->m_x1= t_lcdInfo.m_width - 1;
	}
	if (pcoordinate_pair->m_y1 >= t_lcdInfo.m_height){
		pcoordinate_pair->m_y1= t_lcdInfo.m_height - 1;
	}
	if (pcoordinate_pair->m_x2 >= t_lcdInfo.m_width){
		pcoordinate_pair->m_x2= t_lcdInfo.m_width - 1;
	}
	if (pcoordinate_pair->m_y2 >= t_lcdInfo.m_height){
		pcoordinate_pair->m_y2= t_lcdInfo.m_height - 1;
	}
}


//初始化函数
void GUI_Init(void)
{
	if (s_LcdFd < 0){
		printf("lcd device is not opened\n");
		return;
	}
	s_X						= 0;
	s_Y						= 0;
	LCD_Ioctl(s_LcdFd, ST7735IOC_INIT, NULL);
	LCD_Ioctl(s_LcdFd, ST7735IOC_LCD_INFO, &t_lcdInfo);
	GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_WHITE);
#if (DEBUG > 0)
	printf("width = %d\n", t_lcdInfo.m_width);
	printf("height = %d\n", t_lcdInfo.m_height);
#endif
}

//字符显示函数组
void GUI_DispChar(uint8 c)
{

}
void GUI_DispCharAt(uint8 c, uint32 x, uint32 y)
{

}
void GUI_DispString(const uint8 *s)
{

}
void GUI_DispStringAt(const uint8 *s, uint32 x, uint32 y)
{

}

//坐标函数组
int32 GUI_GotoXY(uint32 x, uint32 y)
{
	int32	ret		= 0;
	if ((x < t_lcdInfo.m_width) || (y < t_lcdInfo.m_height)){
		ret			= -1;
	}else {
		s_X			= x;
		s_Y			= y;
	}
	return ret;
}
int32 GUI_GotoX(uint32 x)
{
	int32	ret		= 0;

	if (x < t_lcdInfo.m_width){
		s_X				= x;
	}else {
		ret			= -1;
	}
	return ret;
}
int32 GUI_GotoY(uint32 y)
{
	int32	ret		= 0;

	if (y < t_lcdInfo.m_height){
		s_Y				= y;
	}else {
		ret			= -1;
	}
	return ret;
}
uint32 GUI_GetX(void)
{
	return s_X;
}
uint32 GUI_GetY(void)
{
	return s_Y;
}

//使用当前的背景色进行清空
void GUI_Clear(void)
{
	LCD_Ioctl(s_LcdFd, ST7735IOC_CLEAR, NULL);
}

//设置前景色
void GUI_SetColor(GUI_COLOR color)
{
	struct color	t_color;

	t_color.m_color		= GUI_Color2Index_565(color);
	if (0 == LCD_Ioctl(s_LcdFd, ST7735IOC_SET_FOREGROUND_COLOR, &t_color)){
		s_Color				= color;
	}
}
GUI_COLOR GUI_GetColor(void)
{
	return s_Color;
}
//设置背景色
void GUI_SetBkColor(GUI_COLOR color)
{
	struct color	t_color;
	t_color.m_color		= GUI_Color2Index_565(color);
	if (0 == LCD_Ioctl(s_LcdFd, ST7735IOC_SET_BACKGROUND_COLOR, &t_color)){
		s_BkColor			= color;
	}
}
GUI_COLOR GUI_GetBkColor(void)
{
	return s_BkColor;
}

//2D绘图函数组
//使用背景色来清除矩形区域
static void GUI_CmdExecRect(int cmd, uint32 x0, uint32 y0, uint32 x1, uint32 y1)
{
	struct coordinate_pair  t_coordinate_pair;

	t_coordinate_pair.m_x1		= x0;
	t_coordinate_pair.m_y1		= y0;
	t_coordinate_pair.m_x2		= x1;
	t_coordinate_pair.m_y2		= y1;
	coordinate_pair_check(&t_coordinate_pair);
#if (DEBUG > 0)
	printf("(%d, %d) - (%d, %d)\n", t_coordinate_pair.m_x1,
			t_coordinate_pair.m_y1,
			t_coordinate_pair.m_x2, t_coordinate_pair.m_y2);
#endif
	LCD_Ioctl(s_LcdFd, cmd, &t_coordinate_pair);
}

static void GUI_CmdExecCircle(int cmd, uint32 x, uint32 y, uint32 r)
{
	struct circle	t_circle;                                     

	t_circle.m_x			= x;
	t_circle.m_y			= y;
	t_circle.m_r			= r;
	LCD_Ioctl(s_LcdFd, cmd, &t_circle);
}

void GUI_ClearRect(uint32 x0, uint32 y0, uint32 x1, uint32 y1)
{
	GUI_CmdExecRect(ST7735IOC_CLEAR_RECTANGLE, x0, y0, x1, y1);
}
//使用前景色来绘制一个像素点
void GUI_DrawPixel(uint32 x, uint32 y)
{
	struct coordinate   t_coordinate; 

	t_coordinate.m_x			= x;
	t_coordinate.m_y			= y;
	coordinate_check(&t_coordinate);
	LCD_Ioctl(s_LcdFd, ST7735IOC_DRAW_LINE, &t_coordinate);
}
//使用前景色来绘制矩形区域
void GUI_DrawRect(uint32 x0, uint32 y0, uint32 x1, uint32 y1)
{
	GUI_CmdExecRect(ST7735IOC_DRAW_RECTANGLE, x0, y0, x1, y1);
}
//使用前景色来填充矩形区域
void GUI_FillRect(uint32 x0, uint32 y0, uint32 x1, uint32 y1)
{
	GUI_CmdExecRect(ST7735IOC_FILL_RECTANGLE, x0, y0, x1, y1);
}
void GUI_DrawLine(uint32 x0, uint32 y0, uint32 x1, uint32 y1)
{
	GUI_CmdExecRect(ST7735IOC_DRAW_LINE, x0, y0, x1, y1);
}
void GUI_DrawCircle(uint32 x, uint32 y, uint32 r)
{
	GUI_CmdExecCircle(ST7735IOC_DRAW_CIRCLE, x, y, r);
}
void GUI_FillCircle(uint32 x, uint32 y, uint32 r)
{
	GUI_CmdExecCircle(ST7735IOC_FILL_CIRCLE, x, y, r);
}
void GUI_FillEllipse(uint32 x0, uint32 y0, uint32 x1, uint32 y1)
{
	GUI_CmdExecRect(ST7735IOC_FILL_ELLIPSE, x0, y0, x1, y1);
}
void GUI_DrawEllipse(uint32 x0, uint32 y0, uint32 x1, uint32 y1)
{
	GUI_CmdExecRect(ST7735IOC_DRAW_ELLIPSE, x0, y0, x1, y1);
}
void GUI_DrawArc(uint32 x, uint32 y, uint32 r, uint32 stangle, uint32 endangle)
{
	struct arc	t_arc;

	t_arc.m_x			= x;
	t_arc.m_y			= y;
	t_arc.m_r			= r;
	t_arc.m_stangle		= stangle;
	t_arc.m_endangle	= endangle;

	LCD_Ioctl(s_LcdFd, ST7735IOC_DRAW_ARC, &t_arc);
}

//绘制bitmap图片
void GUI_DrawBitmap(uint32 x, uint32 y)
{

}


//显示开关函数组
void GUI_DisplayOn(void)
{
	LCD_Ioctl(s_LcdFd, ST7735IOC_BACKLIGHT_ON, NULL);
}
void GUI_DisplayOff(void)
{
	LCD_Ioctl(s_LcdFd, ST7735IOC_BACKLIGHT_OFF, NULL);
}

#if 0
//字体函数组
void GUI_SetFont(const struct GUI_FONT *pfont)
{

}
#endif

#define B_BITS 5
#define G_BITS 6
#define R_BITS 5

#define R_MASK ((1 << R_BITS) -1)
#define G_MASK ((1 << G_BITS) -1)
#define B_MASK ((1 << B_BITS) -1)

static uint16  GUI_Color2Index_565(uint32 Color) 
{  
	int r,g,b;
	r = (Color>> (8  - R_BITS)) & R_MASK;
	g = (Color>> (16 - G_BITS)) & G_MASK;
	b = (Color>> (24 - B_BITS)) & B_MASK;
	//rgb 
	return b + (g << R_BITS) + (r << (G_BITS + R_BITS));
}
