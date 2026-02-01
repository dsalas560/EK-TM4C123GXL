/*
* Timer1A.c
*
* Created on: Mar 20, 2024
* Author: daniel
*/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Timer1A.h"
#include "Timer0A.h"
#include "03_seven_seg.h"
#define PERIODIC_TIME_MS 5 // Adjust this value according to your display refresh
rate
extern uint32_t sysClkFreq; // Assume 8 MHz clock by default
extern int i;
void (*PeriodicTask)(void); // Pointer to the periodic task function
void Timer1A_Init(uint32_t clkFreq, void (*task)(void)) {
PeriodicTask = task; // Save the periodic task function pointer
sysClkFreq = clkFreq;
// Initialize Timer1A for periodic interrupts
SYSCTL_RCGCTIMER_R |= 0x00000002; // Activate Timer1
TIMER1_CTL_R &= ~0x00000001; // Disable Timer1A during setup
TIMER1_CFG_R = 0x00; // Configure for 32-bit timer mode
TIMER1_TAMR_R = 0x2; // Configure for periodic mode, down-count
TIMER1_TAILR_R = (sysClkFreq / 1000) * PERIODIC_TIME_MS - 1; // Set interval load
value for desired time period
TIMER1_TAPR_R = 0; // No prescale
TIMER1_ICR_R = 0x00000001; // Clear Timer1A timeout flag
TIMER1_IMR_R |= 0x00000001; // Enable Timer1A timeout interrupt
NVIC_PRI5_R = (NVIC_PRI5_R & 0x00FFFFFF) | (1 << 29); // Set Timer1A priority level
to 1
NVIC_EN0_R = 1 << 21; // Enable IRQ37 (Timer1A)
TIMER1_CTL_R |= 0x00000001; // Enable Timer1A
//EnableInterrupts();
}
void Timer1A_Handler(void) {
TIMER1_ICR_R = 0x00000001; // Acknowledge Timer1A timeout
//(*PeriodicTask)(); // Call the periodic task function
const static unsigned char digitPattern[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,
0xF8, 0x80, 0x98};
//const static unsigned char digitPattern[] = {0x98,0x80,0xF8,0x82,0x92,0x99,0xB0,
0xA4, 0xF9, 0xC0};
SSI2_write_sevenseg(digitPattern[i]); // write digit pattern to the seven
segments
SSI2_write_sevenseg((1 << i)); // select digit
++i;
if (i>9)
i = 0;
