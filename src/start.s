@ //
@ // ********************************************************
@ // @ * Simple Little Operating System - SLOS
@ // @ *******************************************************

@ ****************************************************************************
@ *
@ * Module     : start.s
@ * Project	: SLOS
@ * Version	: 0.09
@ * Originator	: Andrew N. Sloss
@ * History    :
@ * 
@ * 16th July 2003 Andrew N. Sloss
@ * - added header information
@ *
@ ****************************************************************************

@ *******************************************************
@ * SLOS 2002 
@ *******************************************************

@ *******************************************************
@ * IMPORT 
@ *******************************************************

@************************ Internal Definitions ******************************
@
@
@ to set the mode bits in CPSR for different modes
@
	.set  MODE_USR, 0x10
	.set  MODE_FIQ, 0x11
	.set  MODE_IRQ, 0x12
	.set  MODE_SVC, 0x13
	.set  MODE_ABT, 0x17
	.set  MODE_UND, 0x1B
	.set  MODE_SYS, 0x1F
	.equ  I_F_BIT, 0xC0

	.text
	.code 32
	.align 0
	.section INTERRUPT_VECTOR, "x"
	.global _asm_entry
	.extern debug_init
	.extern led_1_on

_asm_entry:
@
@ Change to SVC mode. Not Already in svc mode 
@
        MRS r0, CPSR
        LDR r1,=0x800a0000 
        STR r0, [r1]
        ADD r0, r0, #1
        LDR r1,=0x800a0004
        STR r0, [r1]
        LDR r0, =0xDEADBEEF
        LDR r1,=0x800a0008
        STR r0, [r1]

	MSR   cpsr_c, #MODE_SVC|I_F_BIT       @ change to SVC mode

        MRS r0, CPSR
       LDR r1,=0x800b0000
        STR r0, [r1]
        ADD r0, r0, #1
        LDR r1,=0x800b0004
        STR r0, [r1]
        LDR r0, =0xDEADBEEF
        LDR r1,=0x800b0008
        STR r0, [r1]



	LDR     r1,vectoraddr			@ save exception base address in VBAR
	MCR     p15, #0, r1, c12, c0, #0
	DSB

 @ ----------------------------------------------------
 @ table offset  
 @ 00 - Reset
 @ 04 - Undefined instructions
 @ 08 - SWI instructions
 @ 0C - prefetch abort
 @ 10 - Data abort
 @ 14 - Reserved
 @ 18 - IRQ interrupts
 @ 1C - FIQ interrupts
 @ ----------------------------------------------------


	.align 5	/* vector base address should be aligned to 16 byte boundary */
vectors:
     LDR     pc,vectorReset
     LDR     pc,vectorUndefined
     LDR     pc,vectorSWI
     LDR     pc,vectorPrefetchAbort
     LDR     pc,vectorDataAbort
     LDR     pc,vectorReserved
     LDR     pc,vectorIRQ
     LDR     pc,vectorFIQ


@ -- Useful address to the PCB tables ------------------ 

ptrPCBTable:
     .word   PCB_Table

@ -- Kernel Jump table ------------------------------ 

vectorReset:
     .word     coreInitialize
vectorUndefined:     
     .word     coreUndefinedHandler
vectorSWI:           
     .word     coreSWIHandler
vectorPrefetchAbort: 
     .word     corePrefetchAbortHandler
vectorDataAbort:
     .word     coreDataAbortHandler
vectorReserved:      
     .word     coreReservedHandler
vectorIRQ:           
     .word     coreIRQHandler
vectorFIQ:           
     .word     coreFIQHandler
vectoraddr:           
     .word     vectors

     .end
