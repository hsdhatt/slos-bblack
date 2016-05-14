/*
 *  ____________________________________________________________________
 * 
 *  Copyright (c) 2002, Andrew N. Sloss, Chris Wright and Dominic Symes
 *  All rights reserved.
 *  ____________________________________________________________________
 * 
 *  NON-COMMERCIAL USE License
 *  
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met: 
 *  
 *  1. For NON-COMMERCIAL USE only.
 * 
 *  2. Redistributions of source code must retain the above copyright 
 *     notice, this list of conditions and the following disclaimer. 
 * 
 *  3. Redistributions in binary form must reproduce the above 
 *     copyright notice, this list of conditions and the following 
 *     disclaimer in the documentation and/or other materials provided 
 *     with the distribution. 
 * 
 *  4. All advertising materials mentioning features or use of this 
 *     software must display the following acknowledgement:
 * 
 *     This product includes software developed by Andrew N. Sloss,
 *     Chris Wright and Dominic Symes. 
 * 
 *   THIS SOFTWARE IS PROVIDED BY THE CONTRIBUTORS ``AS IS'' AND ANY 
 *   EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 *   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
 *   PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE CONTRIBUTORS BE 
 *   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, 
 *   OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 *   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
 *   OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
 *   TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
 *   OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 *   OF SUCH DAMAGE. 
 * 
 *  If you have questions about this license or would like a different
 *  license please email :
 * 
 * 	andrew@sloss.net
 * 
 * 
 */
 
/*****************************************************************************
 * Simple Little Operating System - SLOS
 *****************************************************************************/

/*****************************************************************************
 *
 * Module      : led_driver.c
 * Description : device driver for led on the e7t
 * OS          : SLOS 0.09
 * Platform    : e7t
 * History     :
 *
 * 19th November 2001 Andrew N. Sloss
 * - started working on an example device driver
 *
 * December 2nd 2001 Andrew N. Sloss
 * - added uid generator to the LED driver
 *
 *****************************************************************************/

/*****************************************************************************
 * IMPORT
 *****************************************************************************/

#include "../../devices/ddf_frame.h"
#include "led_driver.h"
#include "../headers/reg.h"

/*****************************************************************************
 * MACROS
 *****************************************************************************/

#define ACCESS_VALUE     0x5AA8
#define LEDPORT    	 0x08000008
#define LEDBANK    	 __REG8(LEDPORT)		

/* -- set */

/*****************************************************************************
 * DATATYPES
 *****************************************************************************/

typedef struct 
{
int uid;
} internal_ledstr;

/*****************************************************************************
 * STATICS
 *****************************************************************************/

typedef unsigned int uint32_t;

unsigned int led_id = 1;

extern led_1_on();
extern led_2_on();
extern led_3_on();
extern led_4_on();
extern led_1_off();
extern led_2_off();
extern led_3_off();
extern led_4_off();


internal_ledstr		diodes[4];

/* -- led_init ----------------------------------------------------------------
 *
 * Description : initalize the LED device driver internal 
 *               data structures and hardware. Set all the
 *               LED's to be zero.
 * 
 * Parameters  : none...
 * Return      : none...
 * Notes       : none...
 *
 */

static inline void setbits_le32(const void* addr, uint32_t pattern)
{
        uint32_t register_value = *(volatile uint32_t *) addr;
        register_value = register_value | pattern;
        *(volatile uint32_t *)addr = register_value;
}

static inline void clrbits_le32(const void* addr, uint32_t pattern)
{
        uint32_t register_value = *(volatile uint32_t *) addr;
        register_value = register_value & ~pattern;
        *(volatile uint32_t *)addr = register_value;
}

/* -- led_init ----------------------------------------------------------------
 *
 * Description : initalize the LED device driver internal 
 *               data structures and hardware. Set all the
 *               LED's to be zero.
 * 
 * Parameters  : none...
 * Return      : none...
 * Notes       : none...
 *
 */

#if 0
static inline void setbits_le32(const void* addr, uint32_t pattern)
{
        uint32_t register_value = *(volatile uint32_t *) addr;
        register_value = register_value | pattern;
        *(volatile uint32_t *)addr = register_value;
}

static inline void clrbits_le32(const void* addr, uint32_t pattern)
{
        uint32_t register_value = *(volatile uint32_t *) addr;
        register_value = register_value & ~pattern;
        *(volatile uint32_t *)addr = register_value;
}
#endif
void led_init(void) 
{
diodes[0].uid = NONE;
diodes[1].uid = NONE;
diodes[2].uid = NONE;
diodes[3].uid = NONE;
led_1_on();

}

/* -- led_open ----------------------------------------------------------------
 *
 * Description : example open on the LED 
 * 
 * Parameters  : unsigned major = 55075 
 *             : unsigned minor = (0=LED0,1=LED1,2=LED2,3=LED3,...,7=LED7)
 * Return      : UID -
 * Notes       : 
 *
 */

UID led_open(unsigned int major,unsigned int minor) 
{
	if (major == DEVICE_LED_BBBLACK) {
		int led_number = 0;
		do {
			if (diodes[led_number].uid == NONE) {
				diodes[led_number].uid = led_id++; 
				return diodes[led_number].uid;
			}

			led_number++;
					
		} while (led_number < 4);

		return DEVICE_IN_USE;
	} else			
		return DEVICE_UNKNOWN;	
}

/* -- led_close ---------------------------------------------------------------
 *
 * Description : example open on the LED 
 * 
 * Parameters  : UID id 
 * Return      : 
 *
 *   DEVICE_SUCCESS - successfully close the device
 *   DEVICE_UNKNOWN - couldn't identify the UID
 *
 * Notes       : 
 *
 */

int led_close(UID id) 
{
 	if (diodes[0].uid == id) {
		diodes[0].uid = NONE;
		return DEVICE_SUCCESS;
    	}

	return DEVICE_UNKNOWN;
}

/* -- led_write_bit -----------------------------------------------------------
 *
 * Description : write a particular bit to an LED 
 * 
 * Parameters  : UID id = 55075 + unit (0..7)
 *	     	: BYTE led_set - least significant bit is used
 * Return	     : none...
 *
 * Notes       : an example of a led write bit
 *
 */

void led_write_byte(UID id, BYTE operation)
{
#if 1
	switch (id) {
		case 1: {
			if (operation == LED_OFF)
				led_1_off();
			else
				led_1_on();
			break;
		}
		case 2: {
			if (operation == LED_OFF)
				led_2_off();
			else
				led_2_on();
			break;
		}
		case 3: {
			if (operation == LED_OFF)
				led_3_off();
			else
				led_3_on();
			break;
		}
		case 4: {
			if (operation == LED_OFF)
				led_4_off();
			else
				led_4_on();
			break;
		}
		default:
			break;
	}
#endif
#if 0

	if (id == 1) {
		if (operation == LED_OFF)
			clrbits_le32((uint32_t *)(SOC_GPIO_1_REGS + GPIO_DATAOUT), LED_PATTERN << LED0);
		else
			setbits_le32((uint32_t *)(SOC_GPIO_1_REGS + GPIO_DATAOUT), LED_PATTERN << LED0);
	}
	if (id == 2) {
		if (operation == LED_OFF)
			clrbits_le32((uint32_t *)(SOC_GPIO_1_REGS + GPIO_DATAOUT), LED_PATTERN << LED1);
		else
			setbits_le32((uint32_t *)(SOC_GPIO_1_REGS + GPIO_DATAOUT), LED_PATTERN << LED1);
	}
#endif
	

return;

}

/* -- led_read_bit ------------------------------------------------------------
 *
 * Description : read a particular bit value 
 * 
 * Parameters  : UID id = 55075 + unit (0..3)
 * Return      : value return error if 255
 *
 * Notes       : an example of a led read bit
 */
#if 0
BYTE led_read_byte(UID id) 
{
	if ((diodes[0].uid == id) && (diodes[0].uid != NONE)) 
		return diodes[0].uid;

	return 255;
}

#endif
BYTE led_read_byte(UID id)
{
	static toggle = 0;
	switch (id) {
		case 1: {
			if (toggle == 1) {
				led_1_off();
				toggle = 0;
			}
			else {
				led_1_on();
				toggle = 1;
			}
			break;
		}
		case 2: {
			if (toggle == 1) {
				led_2_off();
				toggle = 0;
			}
			else {
				led_2_on();
				toggle = 1;
			}
			break;
		}
		case 3: {
			if (toggle == 1) {
				led_3_off();
				toggle = 0;
			}
			else {
				led_3_on();
				toggle = 1;
			}
			break;
		}
		case 4: {
			if (toggle == 1) {
				led_4_off();
				toggle = 0;
			}
			else {
				led_4_on();
				toggle = 1;
			}
			break;
		}
		default:
			break;
	}

	return 1;
}
