#include "tm4c123gh6pm.h"
#include <stdlib.h>
#include <stdint.h>

/**
 * main.c
 */

void delayms(uint32_t ms);
uint32_t count;
int main(void)
{
    SYSCTL_RCGCGPIO_R |= 1U << 5U; /* enable clock to GPIOF */

    GPIO_PORTF_DIR_R |= 0x0E; // Direction set for gpio pins

    GPIO_PORTF_DEN_R |= 0xFF; // Enable GPIO digital function for pins
    while(1){
        GPIO_PORTF_DATA_R |= 1U << 2U;
        delayms(50);
        GPIO_PORTF_DATA_R &= ~(1U << 2U);
        delayms(50);
    }
}


void delayms(uint32_t ms)
{
    int i;
    SYSCTL_RCGCTIMER_R |= 1;     /* enable clock to Timer Block 0 */

    TIMER0_CTL_R = 0;            /* disable Timer before initialization */
    TIMER0_CFG_R = 0x04;         /* 16-bit option */
    TIMER0_TAMR_R = 0x02;        /* periodic mode and down-counter */
    TIMER0_TAILR_R = 16000 - 1;  /* Timer A interval load value register */
    TIMER0_ICR_R = 0x1;          /* clear the TimerA timeout flag*/
    TIMER0_CTL_R |= 0x01;        /* enable Timer A after initialization */

    for(i = 0; i < ms; i++) {
        while ((TIMER0_RIS_R  & 0x1) == 0) ;      /* wait for TimerA timeout flag */
        TIMER0_ICR_R = 0x1;      /* clear the TimerA timeout flag */
    }
}
