.text
.code 32
.global modifyControlCPSR
ADDR_TEMP_STACK:
	.word	0x80090000

modifyControlCPSR:
	LDR	r11, =ADDR_TEMP_STACK
	LDR	r11, [r11]
	STMFD	r11!, {r1, lr}	/* push on the temporary stack */
	MOV	r10, r11
	LDR	r11, =ADDR_TEMP_STACK
	STR	r10, [r11]	/* store the temporary stack pointer to memory */

	MRS	r1,CPSR
	BIC	r1,r1,#0xff
	ORR	r1,r1,r0
	MSR	CPSR_c,r1

	LDR	r11, =ADDR_TEMP_STACK	/* restore the stack pointer in r11 */
	LDR	r11, [r11]
	LDMFD	r11!, {r1, lr}	/* pop the registers on the temporary stack */
	MOV	r10, r11
	LDR	r11, =ADDR_TEMP_STACK
	STR	r10, [r11]	/* store the temporary stack pointer to memory */
	MOV	pc,lr
	.end
