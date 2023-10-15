#include "main.h"
#include "gpioCode.h"
#include "adc.h"
#include "pwmCode.h"
#include "timers.h"
#include "uart_print.h"
#include "sw1_int.h"
#include <stdint.h>
#include <stdbool.h>
#include <common/tm4c123gh6pm.h>

char* float_to_string(float number) {
	static char buffer[6];
    int integerPart = (int)number;
    int decimalPart = (int)((number - integerPart) * 100);

    buffer[0] = (char)('0' + (integerPart / 10));
    buffer[1] = (char)('0' + (integerPart % 10));
    buffer[2] = '.';
    buffer[3] = (char)('0' + (decimalPart / 10));
    buffer[4] = (char)('0' + (decimalPart % 10));
    buffer[5] = (char)('\0');

    return buffer;
}

int main (void) {
	
	GPIOConfigure();
	configureDebounceTimer();
	ADCPinConfigure();
	ADCSampleSequencerConfigure();
	PWMConfigure();
	setup_uart_printer();

	Enable_Interrupts(); //Enable Global Interrupts

	uint16_t duty_cycle_last = 0;
	//int32_t uptime_last = 0;
	//int count = 0;
	while (1) {
		
		uint16_t temp = (uint16_t)((potReading*100)/4095);

		if (temp != duty_cycle_last) {
			PWMSetDutyCycle(temp);
			duty_cycle_last = temp;
		}
		
		if(NEED_PRINT) {
			printlf("The current ADC value is %d and the DC is %s \n", potReading, float_to_string((float)potReading/(float)4095.0f*3.3f));
			NEED_PRINT = false;
		}
		
		printlf("THIS IS VERY LONG SPAM - THE QUICK BROWN FOX JUMPED OVER THE LAZY DOG - the quick brown fox jumped over the lazy dog\n");
	}

	return (0);
}

/*
 * Taken from Lab Assignment
 */
void Disable_Interrupts(void) {
	__asm ("  CPSID	I\n");
}

void Enable_Interrupts(void) {
	__asm ("  CPSIE	I\n");
}
