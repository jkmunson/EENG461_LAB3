#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <driverlib/rom.h>
#include <driverlib/uart.h>
#include <driverlib/sysctl.h>
#include <inc/hw_uart.h>
#include <inc/hw_sysctl.h>
#include <inc/hw_memmap.h>
#include <common/tm4c123gh6pm.h>

#include "uart_print.h"
#include "printf.h"

void setup_uart_printer(void){
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	while(!ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_UART0)){};
	ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, 3);
	ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
	}


void printlf(char format[], ...){
	char buffer[1024];
	va_list args;
	va_start(args, format);
	snprintf(buffer, 1023, format, args);  // Call the original vprintf with va_list
	va_end(args);
	
	for(int i = 0; buffer[i] != '\0'; i++){
		ROM_UARTCharPut(UART0_BASE, buffer[i]);
	}
}
