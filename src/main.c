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

int main (void) {
	
	GPIOConfigure();
	configureDebounceTimer();
	ADCPinConfigure();
	ADCSampleSequencerConfigure();
	PWMConfigure();
	setup_uart_printer();

	Enable_Interrupts(); //Enable Global Interrupts
	
	uint16_t duty_cycle_last = 0;
	
	while (1) {
		
		uint16_t temp = (uint16_t)((potReading*100)/4095);

		if (temp != duty_cycle_last) {
			PWMSetDutyCycle(temp);
			duty_cycle_last = temp;
		}
		
		if(NEED_PRINT) {
			float voltage = ((float)potReading*3.3f)/4095.0f;
			printlf("The current ADC value is %d and the DC is %f \n", potReading, &voltage);
			NEED_PRINT = false;
		}
		
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
