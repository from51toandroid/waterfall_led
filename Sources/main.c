/*---------------------------------------------------------*/
/************************************************************
飞翔科技MC9S12XEP100汽车电子综合开发平台 
E-mail: 2008f.d@163.com
淘宝店：http://fxfreefly.taobao.com
************************************************************/
/*---------------------------------------------------------*/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "stdio.h"

#include "string.h"
#include <stdlib.h>
#include <stdarg.h>





                  


                             
          



#define LEDCPU PORTK_PK4
#define LEDCPU_dir DDRK_DDRK4
#define LED_ON 0
#define LED_OFF 1
#define BUS_CLOCK		   32000000	   //总线频率
#define OSC_CLOCK		   16000000	   //晶振频率


#define LED PORTB
#define LED_dir DDRB

unsigned char data=0x01;


void Ser_Printf (const char *format, ...);


extern void INIT_SCI(void);



void delay_i2c(void) 
{
    unsigned int i,j;
    for( j=0; j<100; j++ ){
      for( i=0; i<60000; i++ ){
	  	;
      }
    }
}


  
	


                        
unsigned char putstring[]="hello freescale mcu\n";
unsigned char *send;


static unsigned char uart_send_a_char(unsigned char ch);

/*************************************************************/
/*                      初始化锁相环                         */
/*************************************************************/
void INIT_PLL(void) 
{
    CLKSEL &= 0x7f;       //set OSCCLK as sysclk
    PLLCTL &= 0x8F;       //Disable PLL circuit
    CRGINT &= 0xDF;
    
    #if(BUS_CLOCK == 40000000) 
      SYNR = 0x44;
    #elif(BUS_CLOCK == 32000000)
      SYNR = 0x43;     
    #elif(BUS_CLOCK == 24000000)
      SYNR = 0x42;
    #endif 

    REFDV = 0x81;         //PLLCLK=2×OSCCLK×(SYNR+1)/(REFDV+1)＝64MHz ,fbus=32M
    PLLCTL =PLLCTL|0x70;  //Enable PLL circuit
    asm NOP;
    asm NOP;
    while(!(CRGFLG&0x08)); //PLLCLK is Locked already
    CLKSEL |= 0x80;        //set PLLCLK as sysclk
}



                        

                         

unsigned char receivedata = 0;

//#pragma  CODE_SEGFLASH_RAM    //?.prm????FLASH_RAM???RAM??
// ?flash???????
#pragma CODE_SEG  MYTEST

                        
int test_func(int a, int b)
{
	int c;  
	__asm nop;
	while(1){
	 a++;
	 if( b==0 ){
         break; 
	 }
	 b--;
	}
	__asm nop;

	__asm nop;
	c=a;
	__asm nop;
	return c;

}
#pragma CODE_SEG DEFAULT

void main(void) 
{       
    int ret;    
    DisableInterrupts; 
    INIT_PLL();
    INIT_SCI();
    //INIT_IIC();
    //INIT_SPI();
    //INIT_FM25040A();
  
    LEDCPU_dir = 1;
    LEDCPU=1;
    LED_dir=0xff;
    EnableInterrupts; 
    __asm nop;
    
    for( ; ; ){
         
         delay_i2c();
		 ret = test_func(11,3);
         Ser_Printf( "testfunc = %d\n", ret );
    }
     
    #if 0
    for( ; ; ){
		 //test_spi();
         delay_i2c();
         receivedata = IIC_receive();     
		 //读取并口数据
         LED = receivedata;    
		 Ser_Printf( "receivedata = %x\n", receivedata );
		 //显示读取到的数据
    } 
    #endif
}








