

;c调用汇编 汇编调用c 测试程序
  XDEF ASM_func_0;
  XREF Ser_Printf;
  
  
DataSec: SECTION
ASMData: DC.B "Hello World in asm"
         DC.B 0dh
         DC.B 0ah
         DC.B 00h
CodeSec: SECTION
ASM_func_0:
 NOP 
   LDD   #ASMData 
   ;获取字符串的基地址
   PSHD     
   CALL Ser_Printf
   PULD
 RTC                     