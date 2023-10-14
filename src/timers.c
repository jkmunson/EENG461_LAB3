#include "timers.h"
#include "main.h"
#include "common/tm4c123gh6pm.h"

void configureTimer (void) {

    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R0; //Enable Run Mode Clock Gating Control for Timer 0

    while (!(SYSCTL_PRTIMER_R & SYSCTL_RCGCTIMER_R0)) {}

    TIMER0_CTL_R &= ~TIMER_CTL_TAEN; //Disable Timer
    TIMER0_CFG_R = TIMER_CFG_32_BIT_TIMER;
    TIMER0_TAMR_R |= TIMER_TAMR_TAMR_PERIOD; //Set Timer to count down periodically
    TIMER0_TAILR_R = 16000 - 1;
    TIMER0_CTL_R |= TIMER_CTL_TAOTE; //Set as an ADC Trigger
    TIMER0_CTL_R |= TIMER_CTL_TAEN; //Enable Timer
}
