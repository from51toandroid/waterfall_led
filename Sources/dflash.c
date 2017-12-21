


#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#define READword(address)     ((unsigned int)(*(volatile unsigned int *__near)(address)))

#define DFLASH_LOWEST_START_PAGE        0x00        //定义data flash的起始页
#define DFLASH_START                    0x00100000  //定义data flash的起始地址
#define DFLASH_PAGE_SIZE                0x0400      //定义data flash的大小为1K.
#define DFLASH_PAGE_WINDOW_START        0x0800      //定义data flash页面窗口的起始地址
#define LEDCPU PORTK_PK4
#define LEDCPU_dir DDRK_DDRK4

unsigned int    Buffer[]={0x1111,0x2222,0x3333,0x4444};//数据缓存区，只能一次写入四个数据
unsigned int    data_Address=0x0000;
unsigned int    date_read[4];

/*************************************************************/
/*                      初始化DFLASH                         */
/*************************************************************/
void DFlash_Init(void)
{
   while(FSTAT_CCIF==0);            //等待正在处理的FLASH操作完成
   FCLKDIV=0x0F;                    //外部晶振为16M.FLASH时钟不超过1M，具体参照手册
   FCNFG=0x00;                      //禁止中断
   while(FCLKDIV_FDIVLD==0);        //等待时钟设置成功
}

/*************************************************************/
/*                     向DFLASH写入数据                      */
/*************************************************************/
void DFlash_Write(word ADDR16)
{
    while(FSTAT_CCIF==0); 
    if(FSTAT_ACCERR)           //判断并清除标志位；
        FSTAT_ACCERR=1;
    if(FSTAT_FPVIOL)           //判断并清除标志位；
        FSTAT_FPVIOL=1;
    FCCOBIX_CCOBIX=0x00; 
    FCCOB=0x1110;         //写入命令和高位地址
    FCCOBIX_CCOBIX=0x01;  //地址后16位
    FCCOB=ADDR16;         //写入低16位地址
    FCCOBIX_CCOBIX=0x02;  //写入第一个数据
    FCCOB=Buffer[0];
    FCCOBIX_CCOBIX=0x03;  //写入第二个数据
    FCCOB=Buffer[1];
    FCCOBIX_CCOBIX=0x04;  //写入第三个数据
    FCCOB=Buffer[2];
    FCCOBIX_CCOBIX=0x05;  //写入第四个数据
    FCCOB=Buffer[3];  
      
    FSTAT_CCIF=1;         //写入执行命令
    while(FSTAT_CCIF==0); //等待执行完毕
}

/*************************************************************/
/*                     由DFLASH读取数据                      */
/*************************************************************/
word DFlash_Read (word destination)
{
    byte   lastepage;          //用于存储EPAGE的值
    byte   epage;              //用于计算EPAGE的值
    unsigned int data;         //读取出的数据

    lastepage = EPAGE;   //保存EPAGE的值

    epage = (byte)((DFLASH_LOWEST_START_PAGE)+(destination >>10));   //计算EPAGE
    EPAGE=epage;                                                     //给EPAGE赋值
 
    data = READword((destination & (DFLASH_PAGE_SIZE - 1)) + DFLASH_PAGE_WINDOW_START);  //读取页面窗口中的数据

    EPAGE= lastepage;       //恢复EPAGE的值

    return(data);
}

/*************************************************************/
/*                    擦除DFLASH的一个分区                   */
/*************************************************************/
void DFlash_Erase(word ADDR16)
{   
  while(FSTAT_CCIF==0);
  if(FSTAT_ACCERR)           //判断并清除标志位；
      FSTAT_ACCERR=1;
  if(FSTAT_FPVIOL)           //判断并清除标志位；
      FSTAT_FPVIOL=1;
  
  FCCOBIX_CCOBIX=0x00;
  FCCOB=0x1210;           //写入擦除命令和高位地址
  FCCOBIX_CCOBIX=0x01;   
  FCCOB=ADDR16;           //写入低16位的地址
  FSTAT_CCIF=1;           //启动执行命令
  while(FSTAT_CCIF==0);   //等待执行完成
}

/*************************************************************/
/*                           主函数                          */
/*************************************************************/
int dflash_main(void)
{
    DisableInterrupts;
    LEDCPU_dir=1;
    LEDCPU=1;
    EnableInterrupts;
    
    DFlash_Init();                  //初始化Flash
    DFlash_Erase(data_Address);     //确保先擦除后写入
    DFlash_Write(data_Address);
    date_read[0]=DFlash_Read(data_Address); 
    date_read[1]=DFlash_Read(data_Address+2); 
    date_read[2]=DFlash_Read(data_Address+4); 
    date_read[3]=DFlash_Read(data_Address+6); 
 
    if(date_read[0]==0x1111&&date_read[1]==0x2222&&date_read[2]==0x3333&&date_read[3]==0x4444){
		//判断数据是否正确
        LEDCPU=0;           
		//点亮指示灯
		return 0;
		
    }else{
        return 1;
	}
 
}


