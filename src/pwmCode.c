#include <stdint.h>
#include "pwmCode.h"
#include "common/tm4c123gh6pm.h"

void PWMConfigure(void) {
    /*
     * Red Led (PF1) is on M1PWM5G2
     */

    SYSCTL_RCGCPWM_R |= SYSCTL_RCGCPWM_R1;
    while(!(SYSCTL_PRPWM_R & SYSCTL_PRPWM_R1)) {}

    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
    while(!(SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5)) {}

    GPIO_PORTF_AFSEL_R |= (1 << 1);
    GPIO_PORTF_PCTL_R |= GPIO_PCTL_PF1_M1PWM5;
    GPIO_PORTF_DEN_R |= (1 << 1);

    PWM1_2_CTL_R = 0x0;
    PWM1_2_GENB_R = PWM_2_GENB_ACTCMPBD_ONE | PWM_2_GENB_ACTLOAD_ZERO | PWM_2_GENB_ACTZERO_ONE;

    PWM1_2_LOAD_R = CYCLES_PER_MS;
    PWM1_2_CMPB_R = 0x0;
    PWMEnable();

}

void PWMSetPeriod(uint16_t cycles_per_period) {
    PWMDisable();
    PWM1_2_LOAD_R = cycles_per_period;
    PWMEnable();
}

void PWMSetDutyCycle(uint8_t duty_cycle) {
    PWMDisable();
    PWM1_2_CMPB_R = CYCLES_PER_MS_DIV_100 * (duty_cycle);
    PWMEnable();
}

void PWMEnable(void) {
    PWM1_2_CTL_R |= PWM_2_CTL_ENABLE;
    PWM1_ENABLE_R |= PWM_ENABLE_PWM5EN;
}

void PWMDisable(void) {
    PWM1_2_CTL_R &= ~PWM_2_CTL_ENABLE;
    PWM1_ENABLE_R &= ~PWM_ENABLE_PWM5EN;
}
