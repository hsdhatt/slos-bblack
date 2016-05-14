@ ****************************************************************************
@ * Simple Little Operating System - SLOS
@ ****************************************************************************

@ ****************************************************************************
@ *
@ * Module     : pcb.s
@ * Project	: SLOS
@ * Version	: 0.09
@ * Originator	: Andrew N. Sloss
@ * History    :
@ * 
@ * 16th July 2003 Andrew N. Sloss
@ * - added header information
@ *
@ ****************************************************************************

.text
     .code 32
     .align 0
     .global pcbSetup
     .global PCB_CurrentTask
     .global PCB_Table
     .global PCB_TopOfIRQStack
     .global PCB_PtrCurrentTask
     .global PCB_PtrNextTask
     .global PCB_PtrTask1
     .global PCB_BottomTask1
     .global PCB_PtrTask2
     .global PCB_BottomTask2
     .global PCB_PtrTask3
     .global PCB_BottomTask3
     .global pcbSetUp
     
/*
@ -- pcbSetup ----------------------------------------------------------------
;
@ Description : PCB setup 
;
@ Parameters  : r0=<thread entry address>
@               r1=<PCB Address>
@               r2=stack offset
@ Return      : setup PCB
@ Notes       : pcbSetUp (UINT *threadEntry,UINT *pcbAddress, UINT *stack)
;
*/

pcbSetUp:
     STR     r0,[r1,#-4]         /* storing the function address */ 
     STR     r0,[r1,#-64]	 /* same */
     SUB     r0, sp, r2		 /* r0 is the stack address */
     LDR     r2,[r2]     	
     STR     r2,[r1,#-8]     	/* stored at -8 */
     MOV     r0,#0x50            /* disable FIQ, enable IRQ and set mode bits to USERMODE 0x10 */
     STR     r0,[r1,#-68] 	/* spsr */
     MOV     pc,lr
     

PCB_Table:
     .word     PCB_PtrTask1
     .word     PCB_PtrTask2
     .word     PCB_PtrTask3

PCB_TopOfIRQStack:
     .word     0x8FA00000 

PCB_PtrCurrentTask:
     .word     0x1

PCB_PtrNextTask:
     .word     0x1

PCB_CurrentTask:
     .word     0x0

#PCB_BottomTask1:
     .skip 68
PCB_PtrTask1:
     .word	0x0
#PCB_BottomTask2:
     .skip 68
PCB_PtrTask2:
     .word     0x0
PCB_BottomTask3:
     .skip 68
PCB_PtrTask3:
	.word 0x0
     .align 4
     .end
