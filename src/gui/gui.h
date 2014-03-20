#ifndef _GUI_HPP_
#define _GUI_HPP_

#include "st7735.h"
#include "font.h"
#include "image.h"

/**********************************
* 
*      typdef  
*
***********************************
*/
typedef unsigned int			u32;
typedef int						i32;
typedef unsigned short int		u16;
typedef short int				i16;
typedef unsigned char			u8;
typedef char					i8;

#ifndef		NULL
#define		NULL			0
#endif

/**********************************
* 
*      Standard colors
*
***********************************
*/
#define GUI_BLUE          0xFF0000
#define GUI_GREEN         0x00FF00
#define GUI_RED           0x0000FF
#define GUI_CYAN          0xFFFF00
#define GUI_MAGENTA       0xFF00FF
#define GUI_YELLOW        0x00FFFF
#define GUI_LIGHTBLUE     0xFF8080
#define GUI_LIGHTGREEN    0x80FF80
#define GUI_LIGHTRED      0x8080FF
#define GUI_LIGHTCYAN     0xFFFF80
#define GUI_LIGHTMAGENTA  0xFF80FF
#define GUI_LIGHTYELLOW   0x80FFFF
#define GUI_DARKBLUE      0x800000
#define GUI_DARKGREEN     0x008000
#define GUI_DARKRED       0x000080
#define GUI_DARKCYAN      0x808000
#define GUI_DARKMAGENTA   0x800080
#define GUI_DARKYELLOW    0x008080
#define GUI_WHITE         0xFFFFFF
#define GUI_LIGHTGRAY     0xD3D3D3
#define GUI_GRAY          0x808080
#define GUI_DARKGRAY      0x404040
#define GUI_BLACK         0x000000

#define GUI_BROWN         0x2A2AA5

#define GUI_INVALID_COLOR 0xFFFFFFF      /* Invalid color - more than 24 bits */


typedef int GUI_COLOR; 

int LCD_Open(const char *path);
void LCD_Close(void);

//初始化函数
void GUI_Init(void);

//字符显示函数组
//void GUI_DispChar(uint8 c);
void GUI_DispCharAt(uint8 c, uint32 x, uint32 y);
//void GUI_DispString(const uint8 *s);
void GUI_DispStringAt(const uint8 *s, uint32 x, uint32 y);

#if 0
//坐标函数组
int32 GUI_GotoXY(uint32 x, uint32 y);
int32 GUI_GotoX(uint32 x);
int32 GUI_GotoY(uint32 y);
uint32 GUI_GetX(void);
uint32 GUI_GetY(void);
#endif

//使用当前的背景色进行清空
void GUI_Clear(void);

//设置前景色
void GUI_SetColor(GUI_COLOR color);
GUI_COLOR GUI_GetColor(void);
//设置背景色
void GUI_SetBkColor(GUI_COLOR color);
GUI_COLOR GUI_GetBkColor(void);

//2D绘图函数组
//使用背景色来清除矩形区域
void GUI_ClearRect(uint32 x0, uint32 y0, uint32 x1, uint32 y1);
//使用前景色来绘制一个像素点
void GUI_DrawPixel(uint32 x, uint32 y);
//使用前景色来绘制矩形区域
void GUI_DrawRect(uint32 x0, uint32 y0, uint32 x1, uint32 y1);
//使用前景色来填充矩形区域
void GUI_FillRect(uint32 x0, uint32 y0, uint32 x1, uint32 y1);
void GUI_DrawLine(uint32 x0, uint32 y0, uint32 x1, uint32 y1);
void GUI_DrawCircle(uint32 x, uint32 y, uint32 r);
void GUI_FillCircle(uint32 x, uint32 y, uint32 r);
void GUI_FillEllipse(uint32 x0, uint32 y0, uint32 x1, uint32 y1);
void GUI_DrawEllipse(uint32 x0, uint32 y0, uint32 x1, uint32 y1);
void GUI_DrawArc(uint32 x, uint32 y, uint32 r, uint32 stangle, uint32 endangle);

//绘制bitmap图片
void GUI_DrawBitmap(const GUI_BITMAP *bitmap, uint32 x, uint32 y);


//显示开关函数组
void GUI_DisplayOn(void);
void GUI_DisplayOff(void);

//字体函数组
void GUI_SetFont(const uint8 *name);
void GUI_AddFont(const GUI_FONT *font);













#endif



