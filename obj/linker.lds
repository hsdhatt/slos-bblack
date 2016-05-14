ENTRY(_asm_entry) /* entry point of the program, first instruction executed */
SECTIONS
{
kernel_text 0x80000000 : {
start.o (INTERRUPT_VECTOR)
init.o (.text)
debug.o (.text)
pcb.o (.text)
undef.o (.text)
swi.o (.text)
prefetch.o (.text)
data.o (.text)
reserved.o (.text)
irq.o (.text)
fiq.o (.text)
scheduler.o (.text)
swi_handler.o (.text)
irq_handler.o (.text)
bringup.o (.text)
cpsr.o (.text)
event_init.o (.text)
tick_service.o (.text)
ddf_io.o (.text)
led_driver.o (.text)
ns16550.o (.text)
serial_driver.o (.text)
watchdog.o (.text)
mutex.o (.text)
cinit.o (.text)
cli_com.o (.text)
cli_readline.o (.text)
cli_shell.o (.text)
cli_serial.o (.text)
uid.o (.text)
/* *(.text)*/
}
kernel_data : { /* global and static variables initialized by the kernel code */
start.o (.data)
init.o (.data)
debug.o (.data)
pcb.o (.data)
undef.o (.data)
swi.o (.data)
prefetch.o (.data)
data.o (.data)
reserved.o (.data)
irq.o (.data)
fiq.o (.data)
scheduler.o (.data)
swi_handler.o (.data)
irq_handler.o (.data)
bringup.o (.data)
cpsr.o (.data)
event_init.o (.data)
tick_service.o (.data)
ddf_io.o (.data)
led_driver.o (.data)
ns16550.o (.data)
serial_driver.o (.data)
watchdog.o (.data)
mutex.o (.data)
cinit.o (.data)
cli_com.o (.data)
cli_readline.o (.data)
cli_shell.o (.data)
cli_serial.o (.data)
uid.o (.data)
} 
kernel_bss : {
start.o (.bss COMMON)
init.o (.bss COMMON)
debug.o (.bss COMMON)
pcb.o (.bss COMMON)
undef.o (.bss COMMON)
swi.o (.bss COMMON)
prefetch.o (.bss COMMON)
data.o (.bss COMMON)
reserved.o (.bss COMMON)
irq.o (.bss COMMON)
fiq.o (.bss COMMON)
scheduler.o (.bss COMMON)
swi_handler.o (.bss COMMON)
irq_handler.o (.bss COMMON)
bringup.o (.bss COMMON)
cpsr.o (.bss COMMON)
event_init.o (.bss COMMON)
tick_service.o (.bss COMMON)
ddf_io.o (.bss COMMON)
led_driver.o (.bss COMMON)
ns16550.o (.bss COMMON)
serial_driver.o (.bss COMMON)
watchdog.o (.bss COMMON)
mutex.o (.bss COMMON)
cinit.o (.bss COMMON)
cli_com.o (.bss COMMON)
cli_readline.o (.bss COMMON)
cli_shell.o (.bss COMMON)
cli_serial.o (.bss COMMON)
uid.o (.bss COMMON)
} /* global and static variable that are not initialized by the kernel code. the compiler initialize this data to 0. */
/*. = ALIGN(8); */
/*. = . + 0x1000;*/ /* 4kB of stack memory */
/*stack_top = .; */ /* stack grows upwards */
/* . = 0x80008000; start of shared region */

shared_text 0x80030000 : {
swi_shared.o (.text)
swiapi_shared.o (.text)
}
shared_data : {
swi_shared.o (.data)
swiapi_shared.o (.data)
}
shared_bss : {
swi_shared.o (.bss COMMON)
swiapi_shared.o (.bss COMMON)
}

/*Pagetable region   0X80010000 - 0x80018000 - 32 KB*/

task1_text 0x80020000 : {
task1_start.o (.text)
}
task1_data : {
task1_start.o (.data)
}
task1_bss : {
task1_start.o (.bss COMMON)
}

task2_text 0x80025000 : {
task2_start.o (.text)
}
task2_data : {
task2_start.o (.data)
}
task2_bss : {
task2_start.o (.bss COMMON)
}

task3_text 0x8002A000 : {
task3_start.o (.text)
}
task3_data : {
task3_start.o (.data)
}
task3_bss : {
task3_start.o (.bss COMMON)
}

}
