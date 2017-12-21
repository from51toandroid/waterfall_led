#include <hidef.h>
//common defines and macros
#include "derivative.h"
//derivative-specific definitions
#include "stdio.h"
#include "string.h"
#include <stdlib.h>
#include <stdarg.h>


#define BUS_CLOCK		   32000000	   
//总线频率
#define OSC_CLOCK		   16000000	   
//晶振频率
#define BAUD 115200


//static int g_printf_switch = 0x01;
static void uart_send_a_char(unsigned char ch);
static void Ser_WrStr(unsigned char* buff);

void INIT_SCI(void) 
{
    SCI0BD  = BUS_CLOCK/16/BAUD;
    //设置SCI0波特率
    SCI0CR1 = 0x00;       
    //设置SCI0为正常模式,八位数据位,无奇偶校验
    SCI0CR2 = 0x08;
    //允许发送数据,禁止中断功能 
}


void SCI_send(unsigned char data) 
{
    while( !SCI0SR1_TDRE ){
      ;
    }  
    //等待发送数据寄存器（缓冲器）为空
    SCI0DRL = data;
}





   

void Ser_Printf (const char *format, ...)
{   
    unsigned char buffer[80 + 1];
    int ret;
    va_list  vArgs;
    //if( g_printf_switch == 0x00 ){
    //    return;
    //}
    va_start(vArgs, format);
    ret = vsprintf((char *)buffer, (char const *)format, vArgs);
    va_end(vArgs);
    Ser_WrStr(( unsigned  char *) buffer);  
}




static void Ser_WrStr(unsigned char* buff)
{
    for( ; *buff; ){
  	     if( *buff == '\n' ){
             uart_send_a_char('\r');
	     }
         uart_send_a_char( *buff++ );
    }
}

static void uart_send_a_char(unsigned char ch)
{
    SCI_send(ch);
}



