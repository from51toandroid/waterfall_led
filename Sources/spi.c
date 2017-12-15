#include <hidef.h>
//common defines and macros
#include "derivative.h"
//derivative-specific definitions
#include "stdio.h"
#include "string.h"
#include <stdlib.h>
#include <stdarg.h>




unsigned char temp_SPI = 0,data_receive = 0;


#define LEDCPU PORTK_PK4

#define HOLD PTJ_PTJ0
#define HOLD_dir DDRJ_DDRJ0
#define CS PTS_PTS7
#define CS_dir DDRS_DDRS7



extern void Ser_Printf (const char *format, ...);


/*************************************************************/
/*                        ��ʱ����                           */
/*************************************************************/
void delay(void) 
{
 unsigned int i;
 for(i=0;i<500;i++);
}

void longdelay(void) 
{
 unsigned int i;
 for(i=0;i<50000;i++);
}


/*************************************************************/
/*                      ��ʼ��SPIģ��                        */
/*************************************************************/
void INIT_SPI(void) 
{
  HOLD_dir = 1;
  CS_dir = 1;

  SPI0CR1 = 0b01010000;   //ʹ��SPI����ֹ�жϣ�ʱ�Ӹ���Ч����λΪ0��
  SPI0CR2 = 0x00;         //SS�ܽ�Ϊ��ͨI/O��˫��ģʽ��
  SPI0BR = 0x70;          //����SPIʱ��Ƶ��Ϊ2MHz;
  CS = 1;
}

/*************************************************************/
/*                       SPI���ͺ���                         */
/*************************************************************/
void SPI_send(unsigned char data) 
{
  while(!SPI0SR_SPTEF);
  SPI0DRL = data; 
}


/*************************************************************/
/*                       SPI���պ���                         */
/*************************************************************/
unsigned char SPI_receive(void) 
{
  unsigned char temp,data;
  while(!SPI0SR_SPIF);
  temp = SPI0SR;
  data = SPI0DRL;
  return(data);
}   


/*************************************************************/
/*                      ��ʼ��FM25040A                       */
/*************************************************************/
void INIT_FM25040A(void) 
{
   HOLD = 1;
   CS = 0;
}


void test_spi(void)
{
    CS = 0;
    SPI_send(0x06);
    delay();
    CS = 1;
                 
    delay();
    CS = 0;
    SPI_send(0x02);
    SPI_send(0x00);
    SPI_send('U');
    //�洢һ���ֽڵ�����
    delay();
    CS = 1;
	
    longdelay();
    CS = 0;
    SPI_send(0x03);
    SPI_send(0x00);
    delay();
      
    SPI_send(0x00);
    delay();
	temp_SPI = SPI_receive();
    //��ȡ����,�Ա�shift register�е����ݴ��뵽SPI0DRL
    data_receive = 0x00;
    data_receive = SPI_receive();
    //��ȡ����
    CS = 1;
    if( data_receive == 'U' ){
        //�ж������Ƿ���ȷ
        LEDCPU = 1;
    }
	Ser_Printf( "data_receive = %c\n", data_receive );
}






