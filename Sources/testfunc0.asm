;else test for asm 




  XDEF ASM_func_1;
  XREF Ser_Printf;
Try EQU 0
DataSec: SECTION
ASMData: DC.B "Hello World in asm"
         DC.B 0dh
         DC.B 0ah
         DC.B 00h
CodeSec: SECTION
ASM_func_1:
   NOP 
   
   IF Try != 0
   LDAA #103
 ELSE
 LDAA #0
 ENDIF
 RTC                           