#include "main.h"
#include "gpioCode.h"
#include "adc.h"
#include "pwmCode.h"
#include "timers.h"
#include "uart_print.h"
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
	int32_t uptime_last = 0;
    while (1) {
		int count = 0;
        uint16_t temp = (uint16_t)((potReading*100)/4095);

        if (temp != duty_cycle_last) {
            PWMSetDutyCycle(temp);
            duty_cycle_last = temp;
        }
		
		if(uptime_seconds - uptime_last > 10) {
			count++;
			printlf("%d: Hello! This is the %d time I've tried to reach you about your car's extended warranty!\r\n", uptime_seconds, count);
			uptime_last = uptime_seconds;
		}
    }

    return (0);
}

/*
 * Taken from Lab Assignment
 */
void Disable_Interrupts(void) {
    __asm ("  CPSID    I\n");
}

void Enable_Interrupts(void) {
    __asm ("  CPSIE    I\n");
}
