/*
* Timer1A.h
*
* Created on: Mar 20, 2024
* Author: danie
*/
#ifndef __TIMER1A_H__
#define __TIMER1A_H__
#include<stdint.h>
extern uint32_t sysClkFreq; // Assume 8 MHz clock by default
#define PERIODIC_TIME_MS 5 // Adjust this value according to your display refresh
rate
extern void (*PeriodicTask)(void); // Pointer to the periodic task function
void Timer1A_Init(uint32_t clkFreq, void (*task)(void));
void Timer1A_Handler(void);
//void Timer1A_Init(uint32_t clkFreq, void (*task)(void));
/*
// Time delay using busy wait
// The delay parameter is in units of the core clock (units of 12.5 nsec for 80 MHz
clock)
void Timer1A_Wait( uint32_t delay );
// Time delay using busy wait
// This assumes 80 MHz system clock
void Timer1A_Wait1ms( uint32_t delay );
*/
#endif /* TIMER1A_H_ */
