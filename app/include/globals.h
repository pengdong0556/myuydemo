/****************************************************************************
* Copyright (C), 2015 瑞邦项目
*
* 文件名: globals.h
* 内容简述:globals全局函数
*       
*
* 文件历史:
* 版本号  日期       作者    说明
* v0.1    2015-10-14 Jason  创建该文件
*
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#ifndef  __GLOBALS_H__
#define  __GLOBALS_H__


#ifdef GLOBALS 
#define EXT
#else
#define EXT extern 
#endif


typedef struct{
    int year; 
    int month; 
    int day;
    int hour;
    int minute;
    int second;
}DATE_TIME;


char *itoa(int value, char *string, int radix);
uint32_t LSB2MSB(uint32_t value);
uint16_t lsb2msb(uint16_t value);
int StringFind(const char *pSrc, const char *pDst);
uint8_t GetComma(uint8_t num, char* buf);
double Get_Double_Number(char *s);
float Get_Float_Number(char *s);
int Get_Int_Number(char *s);
void UTC2BTC(DATE_TIME *dt);
void Int_To_Str(int x, char *Str);




#endif
