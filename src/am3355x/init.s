@ ****************************************************************************
@ * Simple Little Operating System - SLOS
@ ****************************************************************************

@ ****************************************************************************
@ *
@ * Module     : init.s
@ * OS         : SLOS
@ * Version	: 0.09
@ * Originator	: Andrew N. Sloss
@ *
@ * 14 July 2003 Andrew N. Sloss
@ * - separated the pcbSetUp routine
@ *
@ ****************************************************************************

	.text
	.code 32
	.align 0
	.global coreInitialize
	.extern C_Entry
	.extern C_EntryTask2
	.extern C_EntryTask3
	.extern PCB_PtrTask2
	.extern PCB_PtrTask3
	.extern PCB_CurrentTask
	.extern pcbSetUp
	.extern coreCallSWI
	.extern debug_init
	.extern led_1_on 
	.extern led_2_on 
	.extern led_3_on 
	.extern led_4_on 

	.equ IRQ32md, 0x12
	.equ SVC32md, 0x13
	.equ SYS32md, 0x1f
	.equ NoInt,   0xc0

SVC_STACK_START:
	.word	0x80080000	/* SDRAM_BEGIN + 512K. SVC_STACK_SIZE = 128K */

SYS_STACK_START:
	.word	0x80040000	/* SDRAM_BEGIN + 128K. SYS_STACK_SIZE = 512K */

IRQ_STACK_START:
	.word	0x80050000	/* SDRAM_BEGIN + 36K  IRQ_STACK_SIZE = don't know. maybe 28K??*/

TASK2_STACK_START:
	.word	0x80060000	/* SDRAM_BEGIN + 36K  IRQ_STACK_SIZE = don't know. maybe 28K??*/

TASK3_STACK_START:
	.word	0x80070000	/* SDRAM_BEGIN + 36K  IRQ_STACK_SIZE = don't know. maybe 28K??*/
RESET_CPSR_VAL:
	.word	0x800A0000


coreInitialize:

/*
 ------------------------------------------------
 Setup stacks for SVC, IRQ, SYS USER 
 Mode = SVC - ARM boots up in SVC mode with interrupts disabled
 ------------------------------------------------ 
*/
/*
	MRS r0, CPSR
	LDR r1,=0x800a0000
	STR r0, [r1]
	ADD r0, r0, #1
	LDR r1,=0x800a0004
	STR r0, [r1]
	LDR r0, =0xDEADBEEF
	LDR r1,=0x800a0008
	STR r0, [r1]

*/
	ldr sp, SVC_STACK_START
	
	MSR CPSR_c,#NoInt|SYS32md	/* using CPSR_c instead of CPSR avoids altering the condition flags accidentally */

	ldr sp, SYS_STACK_START

	MSR CPSR_c,#NoInt|IRQ32md

	ldr sp, IRQ_STACK_START

	MSR CPSR_c,#NoInt|SVC32md /* back in SVC mode */


/*
 ------------------------------------------------
 Setup Task Process Control Block (PCB).
 Mode = SVC
 ------------------------------------------------
*/

	LDR r0,=C_EntryTask2    
	LDR r1,=PCB_PtrTask2
	LDR r2,=TASK2_STACK_START

	BL pcbSetUp /* (C_EntryTask2, PCB_PtrTask2, TASK2_STACK_START) */

	LDR r0,=C_EntryTask3     
	LDR r1,=PCB_PtrTask3
	LDR r2,=TASK3_STACK_START
	
	BL pcbSetUp

	LDR r0,=PCB_CurrentTask /* set the current ID to TASK1 */

	MOV r1, #0
	STR r1,[r0]

	/* Init the LEDs for debugging */
	BL debug_init	
	BLX   C_Entry
	SUB pc, pc, #0x08                   @ looping   
	
fiq_irq_off_svc:
	SUB pc, pc, #0x08                   @ looping   
	
	.end
