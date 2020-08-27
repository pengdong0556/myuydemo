
/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include "include.h"


/*
* 高低位转换
*/
uint32_t LSB2MSB(uint32_t value)
{
    return (value & 0x000000FFU) << 24 | (value & 0x0000FF00U) << 8 | 
        (value & 0x00FF0000U) >> 8 | (value & 0xFF000000U) >> 24; 
}

/*
* 高低位转换
*/
uint16_t lsb2msb(uint16_t value)
{
	return ( (value & 0xff00) >> 8) | ( (value & 0x00ff ) << 8);
}

int DectoBCD(int Dec, uint8_t *Bcd, int length)
{
     int i;
     int temp;
     for(i=length-1; i>=0; i--)
     {
         temp = Dec%100;
         Bcd[i] = ((temp/10)<<4) + ((temp%10) & 0x0F);
         Dec /= 100;
     }
     return 0;
}


/******************************************************
		整形数据转字符串函数
        char *itoa(int value, char *string, int radix)
		radix=10 标示是10进制	非十进制，转换结果为0;  

	    例：d=-379;
		执行	itoa(d, buf, 10); 后
		
		buf="-379"							   			  
**********************************************************/
char *itoa(int value, char *string, int radix)
{
	int     i, d;
	int     flag = 0;
	char    *ptr = string;

	/* This implementation only works for decimal numbers. */
	if (radix != 10)
	{
	    *ptr = 0;
	    return string;
	}

	if (!value)
	{
	    *ptr++ = 0x30;
	    *ptr = 0;
	    return string;
	}

	/* if this is a negative value insert the minus sign. */
	if (value < 0)
	{
	    *ptr++ = '-';

	    /* Make the value positive. */
	    value *= -1;
	}

	for (i = 10000; i > 0; i /= 10)
	{
	    d = value / i;

	    if (d || flag)
	    {
	        *ptr++ = (char)(d + 0x30);
	        value -= (d * i);
	        flag = 1;
	    }
	}

	/* Null terminate the string. */
	*ptr = 0;

	return string;

} /* NCL_Itoa */

//find string in string, return the first start location or -1 if can not find
int StringFind(const char *pSrc, const char *pDst)
{
	int i, j;
	for (i=0; pSrc[i]!='\0'; i++)	
	{
		if(pSrc[i]!=pDst[0])
			continue;
		j = 0;
		while(pDst[j]!='\0' && pSrc[i+j]!='\0')
		{
			j++;	
			if(pDst[j]!=pSrc[i+j])
				break;
		}
		if(pDst[j]=='\0')
			return i;
	}
	return -1;
}

uint8_t GetComma(uint8_t num, char* buf)
{
	uint8_t i,j = 0;
	int len = strlen(buf);

	for(i=0;i<len;i++)
	{
		if(buf[i] == ',')
			j++;
		if(j == num)
			return i+1;
	}

	return 0;
}

float Str_To_Float(char *buf)
{
	float ret = 0;
	float dat;
	int i = 0,integer = 1;

	char *str = buf;
	while(*str != '\0')
	{
		switch(*str)
		{
			case '0':
				dat = 0;
				break;
			case '1':
				dat = 1;
				break;
			case '2':
				dat = 2;
				break;
			case '3':
				dat = 3;
				break;
			case '4':
				dat = 4;
				break;
			case '5':
				dat = 5;
				break;
			case '6':
				dat = 6;
				break;
			case '7':
				dat = 7;
				break;
			case '8':
				dat = 8;
				break;
			case '9':
				dat = 9;
				break;
			case '.':
				dat = '.';
				break;
		}

		if(dat == '.')
		{
			integer = 0;
			i = 1;
			str++;
			continue;
		}
		if(integer == 1)
		{
			ret = ret*10+dat;
		}
		else
		{
			ret = ret + dat/(10*i);
			i = i*10;
		}
		str++;
	}

	return ret;
}

double Str_To_Double(char *buf)
{
	double ret = 0;
	double dat;
	int i = 0,integer = 1;

	char *str = buf;
	while(*str != '\0')
	{
		switch(*str)
		{
			case '0':
				dat = 0;
				break;
			case '1':
				dat = 1;
				break;
			case '2':
				dat = 2;
				break;
			case '3':
				dat = 3;
				break;
			case '4':
				dat = 4;
				break;
			case '5':
				dat = 5;
				break;
			case '6':
				dat = 6;
				break;
			case '7':
				dat = 7;
				break;
			case '8':
				dat = 8;
				break;
			case '9':
				dat = 9;
				break;
			case '.':
				dat = '.';
				break;
		}

		if(dat == '.')
		{
			integer = 0;
			i = 1;
			str++;
			continue;
		}
		if(integer == 1)
		{
			ret = ret*10+dat;
		}
		else
		{
			ret = ret + dat/(10*i);
			i = i*10;
		}
		str++;
	}

	return ret;
}

double Get_Double_Number(char *s)
{
	char buf[10];
	uint8_t i;
	double ret = 0;

	i = GetComma(1, s);
	i = i - 1;
	if(i > 10) i = 10;
	strncpy(buf, s, i);
	buf[i] = 0;
	ret = Str_To_Double(buf);

	return ret;
}

float Get_Float_Number(char *s)
{
	char buf[10];
	uint8_t i=0;
	float ret=0;

	i = GetComma(1, s);
	i = i - 1;
	if(i > 10) i = 10;
	strncpy(buf, s, i);
	buf[i] = 0;
	ret = Str_To_Float(buf);

	return ret;
}

int Get_Int_Number(char *s)
{
	char buf[10];
	uint8_t i=0;
	int ret=0;

	i = GetComma(1, s);
	i = i - 1;
	if(i > 10) i = 10;
	strncpy(buf, s, i);
	buf[i] = 0;
	ret = atoi(buf);
	
	return ret;
}

void UTC2BTC(DATE_TIME *dt)
{
	dt->second++;
	if(dt->second>59)
	{
		dt->second = 0;
		dt->minute++;
		if(dt->minute>59)
		{
			dt->minute = 0;
			dt->hour++;
		}
	}

	dt->hour = dt->hour+8;
	if(dt->hour>23)
	{
		dt->hour -= 24;
		dt->day += 1;
		if(dt->month == 2 || 
			dt->month == 4 ||
			dt->month == 6 ||
			dt->month == 9 ||
			dt->month == 11)
		{
			if(dt->day>30)
			{
				dt->day = 1;
				dt->month++;
			}
		}
		else
		{
			if(dt->day > 31)
			{
				dt->day = 1;
				dt->month++;
			}
		}
		if(dt->year%4 == 0)
		{
			if(dt->day > 29 && dt->month == 2)
			{
				dt->day = 1;
				dt->month++;
			}
		}
		else
		{
			if(dt->day > 28 && dt->month == 2)
			{
				dt->day = 1;
				dt->month++;
			}
		}
		if(dt->month > 12)
		{
			dt->month -= 12;
			dt->year++;
		}
	}
}

void Int_To_Str(int x, char *Str)
{
	int t, i = 0;
	char *Ptr, Buf[5];
	Ptr = Str;

	if(x < 10)
	{
		*Ptr++ = '0';
		*Ptr++ = x+0x30;
	}
	else
	{
		while(x > 0)
		{
			t = x%10;
			x = x/10;
			Buf[i++] = t + 0x30;
		}
		i--;
		for(;i>=0;i--)
		{
			*(Ptr++) = Buf[i];
		}
	}
	*Ptr = '\0';
	
}


