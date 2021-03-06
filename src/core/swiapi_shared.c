
#include "../am3355x/events/swis.h"
#include "../devices/ddf_frame.h"

#define UINT unsigned int

typedef struct 
{
 unsigned int r[4];
} registerstr;

registerstr rs;

void coreCallSWI (registerstr *r);

/* -- eventIODeviceInit -------------------------------------------------------
 *
 * Description : initializes all internal device driver data structures.
 *               this routine should only be called once. Unless for some
 *               reason you need to reset all the device drivers.
 *
 * Parameters  : none....
 * Return      : none...
 * Notes       :
 * 
 *  This function is called in the core/cinit.c file
 *  
 *    eventIOdeviceInit();
 *
 */

void eventIODeviceInit(void)
{
registerstr tmpReg;

   tmpReg.r[0] = Event_IODeviceInit;
   tmpReg.r[1] = 0;
   tmpReg.r[2] = 0;
   tmpReg.r[3] = 0;
   
   coreCallSWI(&tmpReg);
   
   if (tmpReg.r[0]!=1 || tmpReg.r[1]!=2) { while(999);} 
}

/* -- eventIODeviceOpen -------------------------------------------------------
 *
 * Description : open a device driver
 *
 * Parameters  : UID * - address for UID
 *             : UINT - major device. For example, SERIAL
               : UINT - minor device. For exmaple, COM1
 * Return      : device_treestr * - pointer to device driver
 * Notes       :
 * 
 *   This function is used to open a COM1 of a SERIAL device.
 *   Example: 
 *
 *   handle = eventIODeviceOpen(&UID,SERIAL,COM1);
 *
 *   if (handle==DEVICE_UNKNOWN || handle==DEVICE_IN_USE)
 *   {
 *   canNotOpenDevice(); 
 *   }
 */

device_treestr *eventIODeviceOpen(UID *a, UINT b, UINT c)
{
registerstr tmpReg;

   tmpReg.r[0] = Event_IODeviceOpen;
   tmpReg.r[1] = (UINT) a;
   tmpReg.r[2] = b;
   tmpReg.r[3] = c;
	
  coreCallSWI(&tmpReg);

 
  return (device_treestr *)tmpReg.r[0];
}

/* -- eventIODeviceClose ------------------------------------------------------
 *
 * Description : close a device driver
 *
 * Parameters  : device_treestr * - pointer to device driver
 *             : UID - unique identifer
 * Return      : int
 * Notes       : 
 *
 *  This is used to close an open device
 *  Example: 
 *
 *    if (eventIODeviceClose(handle,id)!=DEVICE_SUCCESS)
 *    {
 *    couldNotCloseDevice();
 *    }
 */

int eventIODeviceClose(device_treestr *a, UID b)
{
registerstr tmpReg;

  tmpReg.r[0] = Event_IODeviceClose;
  tmpReg.r[1] = (UINT) a;
  tmpReg.r[2] = b;
  tmpReg.r[3] = 0;
	
  coreCallSWI(&tmpReg);

  return (int) tmpReg.r[0];
}

int eventIODeviceReadReg(device_treestr *a, UID b)
{
registerstr tmpReg;

  tmpReg.r[0] = Event_IODeviceReadReg;
  tmpReg.r[1] = (UINT) a;
  tmpReg.r[2] = b;
  tmpReg.r[3] = 0;
	
  coreCallSWI(&tmpReg);

  return (int) tmpReg.r[0];
}

/* -- eventIOeviceWriteByte ---------------------------------------------------
 *
 * Description : write a byte to an open device
 *
 * Parameters  : device_treestr * - pointer to device driver
 *             : UID - unique identifier
 *             : UINT - byte store in the lower 8-bits
 * Return      : none...
 * Notes       :
 *
 *   This is used to write a byte to an open device 
 *   Example: eventIODeviceWriteByte(handle,id,c);
 */

void eventIODeviceWriteByte(UINT a,UINT b, UINT c) 
{
	char d = 0x41 + (char)b;
	registerstr tmpReg;
	tmpReg.r[0] = Event_IODeviceWriteByte;
	tmpReg.r[1] = (UINT) a; /*address of led_device*/
	tmpReg.r[2] = b;
	tmpReg.r[3] = c;

	serial_write_byte(1, d); // proc 1 - B proc 2 - C 
	coreCallSWI(&tmpReg);
	serial_write_byte(1, d + 32); // proc 1 - b proc 2 - c
	serial_write_byte(1, '\n'); // proc 1 - b proc 2 - c
	
}

/* -- eventIODeviceReadByte ---------------------------------------------------
 *
 * Description : read a byte from a device
 *
 * Parameters  : device_treestr * - pointer to device driver
 *             : UID - unique identifier
 * Return      : UINT - least significant 8-bits contain the byte
 * Notes       :
 * 
 *  This is used to read a byte from an open device
 *  Example: c = eventIODeviceReadByte(handle,id);
 */

typedef unsigned int uint32_t;
static inline void setbits_le32(const void* addr, uint32_t pattern)
{
        uint32_t register_value = *(volatile uint32_t *) addr;
        register_value = register_value | pattern;
        *(volatile uint32_t *)addr = register_value;
}

UINT eventIODeviceReadByte(device_treestr *a, UID b) 
{
registerstr tmpReg;

  tmpReg.r[0] = Event_IODeviceReadByte;
  tmpReg.r[1] = (UINT) a;
  tmpReg.r[2] = b;
  tmpReg.r[3] = 0;
	
  coreCallSWI(&tmpReg);
  return (UINT) tmpReg.r[0];
}

/* -- eventIODeviceWriteBit ---------------------------------------------------
 *
 * Description : write a bit to a device
 *
 * Parameters  : device_treestr * - pointer to device driver
 *             : UID - unique identifier
 *             : UINT - byte store in the lower 1-bits 
 * Return      : none...
 * Notes       :
 * 
 *  This is used to write a bit to an open device
 *  Example: eventIOdeviceWriteBit(handle,id,bit);
 */

void eventIODeviceWriteBit(device_treestr *a, UID b, UINT c)
{
registerstr tmpReg;

  tmpReg.r[0] = Event_IODeviceWriteBit;
  tmpReg.r[1] = (UINT) a;
  tmpReg.r[2] = b;
  tmpReg.r[3] = c;
	
  coreCallSWI(&tmpReg);
}

/* -- eventIODeviceReadBit ----------------------------------------------------
 *
 * Description : read a bit from a device
 *
 * Parameters  : device_treestr * - pointer to device driver
 *             : UID - unique identifier
 * Return      : UINT - least significant bits contains the bit
 * Notes       :
 * 
 *  This is used to read a bit from an open device
 *  Example: bit = eventIODeviceReadBit(handle,id);
 */

UINT eventIODeviceReadBit(device_treestr *a, UID b) 
{
registerstr tmpReg;

  tmpReg.r[0] = Event_IODeviceReadBit;
  tmpReg.r[1] = (UINT) a;
  tmpReg.r[2] = b;
  tmpReg.r[3] = 0;
	
  coreCallSWI(&tmpReg);
  
  return (UINT) tmpReg.r[0];
}

/* -- eventIODeviceWriteBlock -------------------------------------------------
 *
 * Description : write a block to a device
 *
 * Parameters  : device_treestr * - pointer to device driver
 *             : UID - unique identifier
 *             : void * - point block to write to a device
 * Notes       :
 * 
 *  This is used to write a block to an open device
 *  Example: eventIODeviceWriteBlock(handle,id,&block);
 */

void eventIODeviceWriteBlock(device_treestr *a, UID b, void *c) 
{
registerstr tmpReg;

  tmpReg.r[0] = Event_IODeviceWriteBlock;
  tmpReg.r[1] = (UINT) a;
  tmpReg.r[2] = b;
  tmpReg.r[3] = (UINT) c;
	
  coreCallSWI(&tmpReg);
}

/* -- event_eventIODeviceReadBlock --------------------------------------------
 *
 * Description : read a block from a device
 *
 * Parameters  : device_treestr * - pointer to device driver
 *             : UID - unique identifier
 * Return      : void * - pointer to data block
 * Notes       :
 * 
 *  This is used to read a block from an open device
 *  Example: block = eventIODeviceReadBlock(handle,id);
 */

void *eventIODeviceReadBlock(device_treestr *a, UID b) 
{
registerstr tmpReg;

  tmpReg.r[0] = Event_IODeviceReadBlock;
  tmpReg.r[1] = (UINT) a;
  tmpReg.r[2] = b;
  tmpReg.r[3] = 0;
	
  coreCallSWI(&tmpReg);

  return (void *)tmpReg.r[0];
}

void bringUpTrace(UINT a, UID b)
{
registerstr tmpReg;

  tmpReg.r[0] = BringUp_Trace;
  tmpReg.r[1] = (UINT) a;
  tmpReg.r[2] = b;
  tmpReg.r[3] = 0;

  coreCallSWI(&tmpReg);
}

/* -- bringUpFatalError -------------------------------------------------------
 *
 * Description : very low level fatal error
 *
 * Parameters  : none...
 * Return      : none...
 * Notes       : when this SWI is called SLOS goes into an infinite 
 *               loop. It is designed for basic fatal loops. For PANIC
 *               problems.
 * 
 */

void bringUpFatalError(UID a)
{
registerstr tmpReg;

  tmpReg.r[0] = BringUp_FatalError;
  tmpReg.r[1] = (UINT) a;
  tmpReg.r[2] = 0;
  tmpReg.r[3] = 0;
  
  coreCallSWI(&tmpReg);
}

