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

void setup_uart_printer(void){
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	while(!ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_UART0)){};
	ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, 3);
	ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 921600,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
	}

void print_decimal(int num){
	
	if (num < 0) {
		ROM_UARTCharPut(UART0_BASE, '-');
		num = 0 - num;
	}
	
	char buf[24]; //Large enough to fit any value of num
	
	int places = 0;
	
	do {
		buf[places++] = (char)(48 + (num % 10));
		num = num/10;
	} while (num > 0);
	
	for(; places; places--) {
		ROM_UARTCharPut(UART0_BASE, buf[places-1]);
	}
}

void printlf(char format[], ...){
	va_list args;
	va_start(args, format);
	
	char *str;
	int32_t num;
	
	
	while(ROM_UARTBusy(UART0_BASE)){};
	
	ROM_IntMasterDisable();
	for(int i=0; format[i] != '\0'; i++) {
		while(!ROM_UARTSpaceAvail(UART0_BASE)) {};
		switch(format[i]) {
			case '%' :
				i++;
				switch(format[i]) {
					case 'd' : //decimal number
						num = va_arg(args, int32_t);
						print_decimal(num);
						
					break;
					
					case '\0': // End of string
						ROM_UARTCharPut(UART0_BASE, '%');
						i--; //let the for loop catch this
					break;
					
					case 's': //string
						str = va_arg(args, char*);
						for(int j = 0; str[j] != '\0'; j++) {
							ROM_UARTCharPut(UART0_BASE, str[j]);
						}
					break;
					
					default: //Not recognized
					ROM_UARTCharPut(UART0_BASE, '%');
					ROM_UARTCharPut(UART0_BASE, format[i]);
					
					break;
				}
			break;
				
			default:
				ROM_UARTCharPut(UART0_BASE, format[i]);
			break;
		}
	}
	
	while(ROM_UARTBusy(UART0_BASE)){};
	ROM_IntMasterEnable();
	va_end(args);
}
