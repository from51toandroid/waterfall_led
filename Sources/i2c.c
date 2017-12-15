#include <hidef.h>
//common defines and macros
#include "derivative.h"
//derivative-specific definitions
#include "stdio.h"
#include "string.h"
#include <stdlib.h>
#include <stdarg.h>


/*************************************************************/
/*                        初始化IIC                          */
/*************************************************************/
void INIT_IIC(void) 
{
  IIC0_IBFD = 0x94;   //总线时钟32MHz,设置SCL主频为100KHz
  IIC0_IBCR = 0x80;   //使能IIC模块,禁止中断
  IIC0_IBSR_IBAL = 1; //清除IBAL标志位  
}


/*************************************************************/
/*                  通过IIC由PCF8574读取数据                 */
/*************************************************************/
unsigned char IIC_receive(void) 
{
  unsigned char data;
  IIC0_IBCR_TXAK = 0;               //接收到数据后有应答
  IIC0_IBCR_TX_RX = 1;              //设置单片机为发送模式
  IIC0_IBCR_MS_SL = 1;              //设置单片机为主机模式，产生开始信号

  IIC0_IBDR = 0b01000001;
  while(IIC0_IBSR_IBIF == 0); 
  IIC0_IBSR_IBIF = 1;
  while(IIC0_IBSR_RXAK);

  IIC0_IBCR_TX_RX = 0;             //设置单片机为接收模式
  IIC0_IBCR_TXAK = 1;              //接收到数据后无应答

  data = IIC0_IBDR;                //清空IICD寄存器，准备接收

  while(IIC0_IBSR_IBIF == 0); 
  IIC0_IBSR_IBIF = 1;
  IIC0_IBCR_MS_SL = 0;

  data = IIC0_IBDR;                //读取接收到的数据
  return(data);
}





