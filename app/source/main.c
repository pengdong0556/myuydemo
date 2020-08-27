/*********************************************************************************************************
*
* File                : main.c
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : V1.0
* By                  : 
*
*                                  (c) Copyright 2005-2011, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "include.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/


OS_EVENT *xMUTEX;



__align(8) static  OS_STK App_TaskStartStk[APP_TASK_START_STK_SIZE];
__align(8) static  OS_STK Task_INSTRUMENTStk[Task_INSTRUMENT_STK_SIZE];




static  void  App_TaskStart(void* p_arg);
static  void  App_EventCreate(void);




/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{ 

	
 
	
	__disable_irq();
  BSP_Init();
	OSInit();
	OSTaskCreate((void (*) (void *)) App_TaskStart,(void *) 0,
	(OS_STK *) &App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],
	(INT8U) APP_TASK_START_PRIO);
  OSTimeSet(0);
	OSStart();

	return (0);

}
static  void App_TaskCreate(void)
{
	
	    
	
	    OSTaskCreateExt(Task_Instrument_Run,(void *)0,
			(OS_STK *)&Task_INSTRUMENTStk[Task_INSTRUMENT_STK_SIZE-1],
			Task_INSTRUMENT_PRIO,Task_INSTRUMENT_PRIO,(OS_STK *)&Task_INSTRUMENTStk[0],
			Task_INSTRUMENT_STK_SIZE,(void *)0,OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR);

	
	
	
	
}


static  void  App_TaskStart(void* p_arg)
{
	(void) p_arg;


	/* Initialize the SysTick.       */
	 OS_CPU_SysTickInit();
		
#if (OS_TASK_STAT_EN > 0)
	 OSStatInit();        	/* Determine CPU capacity.    */
#endif
	
	 App_EventCreate();
   App_TaskCreate();
   TRACE("system init ok\r\n");
	 OSTaskSuspend(APP_TASK_START_PRIO);	     //suspend but not delete
	
}

static  void  App_EventCreate(void)
{ 
	INT8U err;
	xMUTEX = OSMutexCreate(MUTEX_PRIO,&err);
}
/*******************************************************************************
* Function Name  : Delay
* Description    : Delay Time
* Input          : - nCount: Delay Time
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void  Delay (uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}



#ifdef   APP_DBG_USED
void  App_Printf(char *format, ...)
{
    char  buf_str[250 + 1];
    INT8U err;
	  va_list   v_args;
    va_start(v_args, format);
    (void)vsnprintf((char       *)&buf_str[0],
                   (size_t      ) sizeof(buf_str),
                   (char const *) format,
                                  v_args);
    va_end(v_args);
 
		OSMutexPend(xMUTEX, 10, &err);							 
		
		//if (OS_ERR_NONE  == err)
		//{
		printf("%s", buf_str);
		OSMutexPost(xMUTEX);
		//}
		
									 
}
#endif	

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */

void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
	
  while (1)
  {
  }
}
#endif

/*************************************** END OF FILE *************************************/
