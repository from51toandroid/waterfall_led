

;c���û�� ������c ���Գ���
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
   ;��ȡ�ַ����Ļ���ַ
   PSHD     
   CALL Ser_Printf
   PULD
 RTC                     