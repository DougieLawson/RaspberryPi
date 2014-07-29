/*
  GPIO_Pin_green = 4
  GPIO_Pin_red = 9
  GPIO_Pin_yellow = 22
  GPIO_Pin_buzz = 8
  GPIO_Pin_button = 7
*/

.section .init
.globl _start
_start:

/*
 Initialise GPIO pins
*/

  ldr r0,=0x20200000  @ base addr for GPIO
  mov r1,#1 
  lsl r1,#27          @ Set GPIO09 as output
  mov r2,#1
  lsl r2,#12          @ Set GPIO04 as output
  add r1,r1,r2
  str r1,[r0,#0x00]   @ GPIO register GPFSEL0
  mov r1,#1
  lsl r1,#6           @ Set GPIO22 as output
  str r1,[r0,#0x08]   @ GPIO register GPFSEL2

/*
 Set up R1,R2 & R3 to switch bit on and off
*/

  mov r1,#1
  lsl r1,#22          @ GPIO22 yellow
  mov r2,#1 
  lsl r2,#9           @ GPIO09 red
  mov r3,#1
  lsl r3,#4           @ GPIO04 green

mainloop: 

  mov r4,r2           @ RED == Reg2
  bl  set_wait
  mov r4,r2           @ RED == Reg2
  bl  set_wait
  mov r4,r1           @ YELLOW == Reg1
  add r4,r4,r2        @ RED & YELLOW
  bl  set_wait
  mov r4,r3           @ GREEN == Reg3
  bl  set_wait
  mov r4,r3           @ GREEN == Reg3
  bl  set_wait
  mov r4,r1           @ YELLOW == Reg1
  bl  set_wait

  b   mainloop

set_wait:

  str r4,[r0,#0x1C]   @ Set GPSET0 register 

  mov r6,#5           @ Number of times round the 3F0000 loop

counter:

  mov r5,#0x3F0000    @ delay counter

waitloop:

  sub r5,#1
  cmp r5,#0
  bne waitloop
  sub r6,#1
  cmp r6,#0
  bne counter

resetall:

  mov r4,r1            
  add r4,r4,r2
  add r4,r4,r3
  str r4,[r0,#0x28]   @ Set GPCLR0 register       
  bx  lr
