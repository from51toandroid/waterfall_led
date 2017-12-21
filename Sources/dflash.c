


#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#define READword(address)     ((unsigned int)(*(volatile unsigned int *__near)(address)))

#define DFLASH_LOWEST_START_PAGE        0x00        //����data flash����ʼҳ
#define DFLASH_START                    0x00100000  //����data flash����ʼ��ַ
#define DFLASH_PAGE_SIZE                0x0400      //����data flash�Ĵ�СΪ1K.
#define DFLASH_PAGE_WINDOW_START        0x0800      //����data flashҳ�洰�ڵ���ʼ��ַ
#define LEDCPU PORTK_PK4
#define LEDCPU_dir DDRK_DDRK4

unsigned int    Buffer[]={0x1111,0x2222,0x3333,0x4444};//���ݻ�������ֻ��һ��д���ĸ�����
unsigned int    data_Address=0x0000;
unsigned int    date_read[4];

/*************************************************************/
/*                      ��ʼ��DFLASH                         */
/*************************************************************/
void DFlash_Init(void)
{
   while(FSTAT_CCIF==0);            //�ȴ����ڴ����FLASH�������
   FCLKDIV=0x0F;                    //�ⲿ����Ϊ16M.FLASHʱ�Ӳ�����1M����������ֲ�
   FCNFG=0x00;                      //��ֹ�ж�
   while(FCLKDIV_FDIVLD==0);        //�ȴ�ʱ�����óɹ�
}

/*************************************************************/
/*                     ��DFLASHд������                      */
/*************************************************************/
void DFlash_Write(word ADDR16)
{
    while(FSTAT_CCIF==0); 
    if(FSTAT_ACCERR)           //�жϲ������־λ��
        FSTAT_ACCERR=1;
    if(FSTAT_FPVIOL)           //�жϲ������־λ��
        FSTAT_FPVIOL=1;
    FCCOBIX_CCOBIX=0x00; 
    FCCOB=0x1110;         //д������͸�λ��ַ
    FCCOBIX_CCOBIX=0x01;  //��ַ��16λ
    FCCOB=ADDR16;         //д���16λ��ַ
    FCCOBIX_CCOBIX=0x02;  //д���һ������
    FCCOB=Buffer[0];
    FCCOBIX_CCOBIX=0x03;  //д��ڶ�������
    FCCOB=Buffer[1];
    FCCOBIX_CCOBIX=0x04;  //д�����������
    FCCOB=Buffer[2];
    FCCOBIX_CCOBIX=0x05;  //д����ĸ�����
    FCCOB=Buffer[3];  
      
    FSTAT_CCIF=1;         //д��ִ������
    while(FSTAT_CCIF==0); //�ȴ�ִ�����
}

/*************************************************************/
/*                     ��DFLASH��ȡ����                      */
/*************************************************************/
word DFlash_Read (word destination)
{
    byte   lastepage;          //���ڴ洢EPAGE��ֵ
    byte   epage;              //���ڼ���EPAGE��ֵ
    unsigned int data;         //��ȡ��������

    lastepage = EPAGE;   //����EPAGE��ֵ

    epage = (byte)((DFLASH_LOWEST_START_PAGE)+(destination >>10));   //����EPAGE
    EPAGE=epage;                                                     //��EPAGE��ֵ
 
    data = READword((destination & (DFLASH_PAGE_SIZE - 1)) + DFLASH_PAGE_WINDOW_START);  //��ȡҳ�洰���е�����

    EPAGE= lastepage;       //�ָ�EPAGE��ֵ

    return(data);
}

/*************************************************************/
/*                    ����DFLASH��һ������                   */
/*************************************************************/
void DFlash_Erase(word ADDR16)
{   
  while(FSTAT_CCIF==0);
  if(FSTAT_ACCERR)           //�жϲ������־λ��
      FSTAT_ACCERR=1;
  if(FSTAT_FPVIOL)           //�жϲ������־λ��
      FSTAT_FPVIOL=1;
  
  FCCOBIX_CCOBIX=0x00;
  FCCOB=0x1210;           //д���������͸�λ��ַ
  FCCOBIX_CCOBIX=0x01;   
  FCCOB=ADDR16;           //д���16λ�ĵ�ַ
  FSTAT_CCIF=1;           //����ִ������
  while(FSTAT_CCIF==0);   //�ȴ�ִ�����
}

/*************************************************************/
/*                           ������                          */
/*************************************************************/
int dflash_main(void)
{
    DisableInterrupts;
    LEDCPU_dir=1;
    LEDCPU=1;
    EnableInterrupts;
    
    DFlash_Init();                  //��ʼ��Flash
    DFlash_Erase(data_Address);     //ȷ���Ȳ�����д��
    DFlash_Write(data_Address);
    date_read[0]=DFlash_Read(data_Address); 
    date_read[1]=DFlash_Read(data_Address+2); 
    date_read[2]=DFlash_Read(data_Address+4); 
    date_read[3]=DFlash_Read(data_Address+6); 
 
    if(date_read[0]==0x1111&&date_read[1]==0x2222&&date_read[2]==0x3333&&date_read[3]==0x4444){
		//�ж������Ƿ���ȷ
        LEDCPU=0;           
		//����ָʾ��
		return 0;
		
    }else{
        return 1;
	}
 
}


