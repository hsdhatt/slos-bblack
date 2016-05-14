        
	/* for led debugging. */

	.equ CM_PER_GPIO1_CLKCTRL, 0x44e000AC
        .equ GPIO1_OE, 0x4804C134
        .equ GPIO1_CLEARDATAOUT, 0x4804C190
        .equ GPIO1_SETDATAOUT, 0x4804C194
        .equ GPIO1_DATAOUT, 0x4804C13C

	.text
	.code 32
	.align 0
	.global debug_init
	.global led_1_on
	.global led_2_on
	.global led_3_on
	.global led_4_on
	.global led_1_off
	.global led_2_off
	.global led_3_off
	.global led_4_off

debug_init:
	PUSH {r0, r1}	/* because going to modify r0, r1 in this function */ 

	/* set clock for GPIO1, TRM 8.1.12.1.29 */
	LDR r0,=CM_PER_GPIO1_CLKCTRL
	LDR r1,=0x40002
	STR r1,[r0]

	/* set pin 21 for output, led USR0, TRM 25.3.4.3 */
	LDR r0,=GPIO1_OE
	LDR r1,[r0]
	BIC r1,r1, #(1<<21)
	STR r1,[r0]

	/* set pin 22 for output, led USR1, TRM 25.3.4.3 */
	LDR r0,=GPIO1_OE
	LDR r1,[r0]
	BIC r1,r1, #(1<<22)
	STR r1,[r0]

	/* set pin 23 for output, led USR2, TRM 25.3.4.3 */
	LDR r0,=GPIO1_OE
	LDR r1,[r0]
	BIC r1,r1, #(1<<23)
	STR r1,[r0]

	/* set pin 24 for output, led USR3, TRM 25.3.4.3 */
	LDR r0,=GPIO1_OE
	LDR r1,[r0]
	BIC r1,r1, #(1<<24)
	STR r1,[r0]

	POP {r0, r1}
	MOV pc,lr	/* Return to caller */

led_1_on:
	PUSH {r0, r2}
	
	/* logical 1 turns on the led, TRM 25.3.4.2.2.2 */
	LDR r0,=GPIO1_SETDATAOUT
	LDR r2,[r0]
	ORR r2,r2, #(1<<21)
	STR r2,[r0]

	POP {r0, r2}
	MOV pc,lr	/* Return to Caller*/

led_1_off:
	PUSH {r0, r2}
        
	/* logical 0 turns off the led, TRM 25.3.4.2.2.2 */
	LDR r0,=GPIO1_DATAOUT
	LDR r2,[r0]
	BIC r2,r2, #(1<<21)
	STR r2,[r0]
	
	POP {r0, r2}
	MOV pc,lr	/* Return to Caller*/

led_2_on:
	PUSH {r0, r2}
	
	/* logical 1 turns on the led, TRM 25.3.4.2.2.2 */
	LDR r0,=GPIO1_SETDATAOUT
	LDR r2,[r0]
	ORR r2,r2, #(1<<22)
	STR r2,[r0]

	POP {r0, r2}
	MOV pc,lr	/* Return to Caller*/

led_2_off:
	PUSH {r0, r2}
        
	/* logical 0 turns off the led, TRM 25.3.4.2.2.2 */
	LDR r0,=GPIO1_DATAOUT
	LDR r2,[r0]
	BIC r2,r2, #(1<<22)
	STR r2,[r0]
	
	POP {r0, r2}
	MOV pc,lr	/* Return to Caller*/

led_3_on:
	PUSH {r0, r2}
	
	/* logical 1 turns on the led, TRM 25.3.4.2.2.2 */
	LDR r0,=GPIO1_SETDATAOUT
	LDR r2,[r0]
	ORR r2,r2, #(1<<23)
	STR r2,[r0]

	POP {r0, r2}
	MOV pc,lr	/* Return to Caller*/

led_3_off:
	PUSH {r0, r2}
        
	/* logical 0 turns off the led, TRM 25.3.4.2.2.2 */
	LDR r0,=GPIO1_DATAOUT
	LDR r2,[r0]
	BIC r2,r2, #(1<<23)
	STR r2,[r0]
	
	POP {r0, r2}
	MOV pc,lr	/* Return to Caller*/

led_4_on:
	PUSH {r0, r2}
	
	/* logical 1 turns on the led, TRM 25.3.4.2.2.2 */
	LDR r0,=GPIO1_SETDATAOUT
	LDR r2,[r0]
	ORR r2,r2, #(1<<24)
	STR r2,[r0]

	POP {r0, r2}
	MOV pc,lr	/* Return to Caller*/

led_4_off:
	PUSH {r0, r2}
        
	/* logical 0 turns off the led, TRM 25.3.4.2.2.2 */
	LDR r0,=GPIO1_DATAOUT
	LDR r2,[r0]
	BIC r2,r2, #(1<<24)
	STR r2,[r0]
	
	POP {r0, r2}
	MOV pc,lr	/* Return to Caller*/

	.end
