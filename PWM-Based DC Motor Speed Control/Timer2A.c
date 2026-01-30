#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Timer2A.h"
uint16_t last=0;
uint16_t current=0;
uint16_t period=0;
// Using PB0 for input capture (T2CCP0)
void Timer2A_Init(){
SYSCTL_RCGCTIMER_R |= 0x00000004; // Activate Timer2
SYSCTL_RCGCGPIO_R |= 0x00000002; // Activate Port B
GPIO_PORTB_DEN_R |= 0x01; // Enable digital I/O on PB0
GPIO_PORTB_AFSEL_R |= 0x01; // Enable alternate function on PB0
GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xFFFFFFF0) | 0x00000007; // Enable
T2CCP0
TIMER2_CTL_R &= ~TIMER_CTL_TAEN; // Disable Timer2A during setup
TIMER2_CFG_R = TIMER_CFG_16_BIT; // Configure for 16-bit timer mode
TIMER2_TAMR_R = TIMER_TAMR_TACMR | TIMER_TAMR_TAMR_CAP; // Configure for
capture mode
TIMER2_CTL_R &= ~(TIMER_CTL_TAEVENT_POS | 0xC ); // Configure for rising-edge event
TIMER2_TAILR_R = 0x0000FFFF; // Start value
TIMER2_IMR_R |= TIMER_IMR_CAEIM; // Enable capture match interrupt
TIMER2_ICR_R = TIMER_ICR_CAECINT; // Clear Timer2A capture match flag
TIMER2_CTL_R |= TIMER_CTL_TAEN; // Enable Timer2A
NVIC_PRI5_R = (NVIC_PRI5_R & 0x00FFFFFF) | 0x40000000; // Timer2A = Priority 2
NVIC_EN0_R = 0x00800000; // Enable interrupt 23 in NVIC
TIMER2_TAR_R = 0x40032048;
EnableInterrupts();
return;
}
void Timer2A_Handler(){
TIMER2_ICR_R = TIMER_ICR_CAECINT; // Acknowledge Timer2A capture
// TODO: Calculate the period or pulse length of the DC motor's encoder here
current = TIMER2_TAR_R; //this is the clock cycle count for the current capture match event
period = last - current;
last = current; // To continually get an accurate measurement and move with the pulses we set
the current value to the last value and await the new current.
return;
}
