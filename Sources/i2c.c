#include <hidef.h>
//common defines and macros
#include "derivative.h"
//derivative-specific definitions
#include "stdio.h"
#include "string.h"
#include <stdlib.h>
#include <stdarg.h>


/*************************************************************/
/*                        ��ʼ��IIC                          */
/*************************************************************/
void INIT_IIC(void) 
{
  IIC0_IBFD = 0x94;   //����ʱ��32MHz,����SCL��ƵΪ100KHz
  IIC0_IBCR = 0x80;   //ʹ��IICģ��,��ֹ�ж�
  IIC0_IBSR_IBAL = 1; //���IBAL��־λ  
}


/*************************************************************/
/*                  ͨ��IIC��PCF8574��ȡ����                 */
/*************************************************************/
unsigned char IIC_receive(void) 
{
  unsigned char data;
  IIC0_IBCR_TXAK = 0;               //���յ����ݺ���Ӧ��
  IIC0_IBCR_TX_RX = 1;              //���õ�Ƭ��Ϊ����ģʽ
  IIC0_IBCR_MS_SL = 1;              //���õ�Ƭ��Ϊ����ģʽ��������ʼ�ź�

  IIC0_IBDR = 0b01000001;
  while(IIC0_IBSR_IBIF == 0); 
  IIC0_IBSR_IBIF = 1;
  while(IIC0_IBSR_RXAK);

  IIC0_IBCR_TX_RX = 0;             //���õ�Ƭ��Ϊ����ģʽ
  IIC0_IBCR_TXAK = 1;              //���յ����ݺ���Ӧ��

  data = IIC0_IBDR;                //���IICD�Ĵ�����׼������

  while(IIC0_IBSR_IBIF == 0); 
  IIC0_IBSR_IBIF = 1;
  IIC0_IBCR_MS_SL = 0;

  data = IIC0_IBDR;                //��ȡ���յ�������
  return(data);
}





