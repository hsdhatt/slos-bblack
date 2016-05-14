@ ****************************************************************************
@ * Simple Little Operating System - SLOS
@ ****************************************************************************

@ ****************************************************************************
@ *
@ * Module     : swi.s
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
	.global coreCallSWI

/*
 ****************************************************************************
 * kernel swi handler event
 ****************************************************************************


  void coreCallSWI(registerstr *r0)
  User/Sys Mode
*/

coreCallSWI:
	STMFD r13!,{r0-r4,r12,lr}	/* save the corruptible registers */
	MOV r4,r0			/* r0 contains the address of tmpreg */

	LDMIA r4,{r0-r3}			/* 
					 * Prepare the arguments. r0 - typeofiocall, 
					 * Event)IODeviceInit  - r1 - 0, r2 - 0 , r3 - 0  
					 * Event_IODeviceOpen  - r1 - addr of UID, r2 - device major, r3 - minor
					 * Event_IODeviceWrite - r1 - addr of devicestr, r2 - UID, r3 - byte to write
					 * Event_IODeviceRead  - r1 - addr of devicestr, on return r1 - read byte , r2 - UID, r3 - 0 
					 */
	SVC 5075
	STMIA   r4,{r0-r3}			
	LDMFD   r13!,{r0-r4,r12,pc}	/* restore all the reg values */

