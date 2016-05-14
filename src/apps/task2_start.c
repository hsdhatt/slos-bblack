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
/**************************************************************
 * Simple Little Operating System - SLOS
 **************************************************************/

/***************************************************************
 *
 * Module       : _e7t_task2_start.c
 * Description  : This is an application that continuous runs
 *                until interrupted by a periodic interrupt
 * Platform     : e7t
 * History      :
 *		
 * 2000-03-26 Andrew N. Sloss
 * - implememtented on ARM Evaluator7T
 *
 * Notes        : none...
 *
 ***************************************************************/

/***************************************************************
 * IMPORT
 ***************************************************************/
#include "../core/mutex.h"

#include "../headers/api_types.h"
#include "../devices/ddf_io.h"
#include "../am3355x/devices/ddf_types.h"

#include "../am3355x/events/swis.h"
#include "../headers/api_device.h"


/*****************************************************************************
 * ROTUINES
 *****************************************************************************/
#define LED 0
device_treestr *led_host2;
UID led2;

/* -- openOrangeLED -----------------------------------------------------------
 *
 * Description   : opens the communication port to the red LED
 *
 * Parameters    : none..
 * Return        : BOOLean - successful | unsucessful 
 * Notes         :
 *
 */

typedef unsigned int uint32_t;
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

BOOL openLED2(void) 
{ 

	led_host2 = eventIODeviceOpen(&led2, DEVICE_LED_BBBLACK, LED);

	/* check that a device driver is found ... */
	if (led_host2 == 0)
		return FALSE;

	return TRUE; 
}

/* -- switchOnOrangeLED -------------------------------------------------------
 *
 * Description  : switches on the Orange LED
 *
 * Parameter    : none...
 * Return       : none...
 * Notes	:
 *
 */

void switchLED2(BYTE op)
{
	eventIODeviceWriteByte(led_host2, led2, (BYTE)op);
}

/* -- switchOffOrangeLED ------------------------------------------------------
 *
 * Description  : switches off the Orange LED
 *
 * Parameter    : none...
 * Return       : none...
 * Notes	:
 *
 */

/* -- C_EntryTask2 ------------------------------------------------------------
 *
 * Description : C entry point for task 2
 * 
 * Parameters  : none...
 * Return      : none...
 * Other       :
 *
 * This task executes continuously
 *
 */

void C_EntryTask2(void)
{
	volatile int delay, toggle = 0;

//	while(1);

	if (openLED2() == TRUE) {
		while(1) {
			for (delay=0; delay<(0x0010ffff/32); delay++)
				;

			if (toggle % 2 == 0)
				switchLED2(0);
			else
				switchLED2(1);

			toggle++;
		}
	}

	/* error loop */
	while (1)
		delay=0xBEEF0002;
}

