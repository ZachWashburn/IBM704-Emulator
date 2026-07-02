; THIS IS A PROGRAM WRITTEN BY ME
; IT DEMONSTATES ASSEMBLY ON THE
; EMULATED IBM 704:
.code: 0x00
LDQ 0x3000    ; load 0x3000 ( DEADBEEF ) into MQ
STQ 0x4000    ; store MQ into C(Y) (0x4000)
CLA 0x4000    ; clear AC and add (0x4000)... basically load that VAL into AC
SUB 0x3001    ; subtract 0xBEEF from 0xDEADBEEF, leaving 0xDEAD0000 in AC 
STO 0x4000    ; store AC into 0x4000
LDQ 0x4000    ; Load 0xDEAD0000 into MQ 
.data: 0x3000
0xDEADBEEF
0xBEEF

; HEAP
.data: 0x4000
0x0000

