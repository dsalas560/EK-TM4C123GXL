#include <stdint.h>
#include "os.h"
#include "tm4c123gh6pm.h"
#include "Timer0A.h"
#include "Timer1A.h"
#include "Timer2A.h"
#include "03_seven_seg.h"
#include "SSI2.h"
#include "PLL.h"
#define TIMESLICE 80000 // TODO: define the thread switch time in number of SysTick
counts (bus clock cycles at 8 MHz)
uint32_t Count1; // number of times thread1 loops
uint32_t Count2; // number of times thread2 loops
uint32_t Count3; // number of times thread3 loops
void Task1(void){
Count1 = 0;
for(;;){
Count1++;
GPIO_PORTF_DATA_R = (GPIO_PORTF_DATA_R & ~0x0E) | (0x01<<1); // Show red
}
}
void Task2(void){
Count2 = 0;
for(;;){
Count2++;
GPIO_PORTF_DATA_R = (GPIO_PORTF_DATA_R & ~0x0E) | (0x02<<1); // Show blue
}
}
void Task3(void){
Count3 = 0;
for(;;){
Count3++;
GPIO_PORTF_DATA_R = (GPIO_PORTF_DATA_R & ~0x0E) | (0x03<<1); // Show red + blue
= purple/magenta
}
}
/*
void PeriodicTask(void) {
static int taskIndex = 1; // Variable to keep track of the current task index
// Call Task1, Task2, or Task3 based on the current task index
switch (taskIndex) {
case 1:
Task1(); // Call Task1
break;
case 2:
Task2(); // Call Task2
break;
case 3:
Task3(); // Call Task3
break;
default:
// Handle invalid task index
break;
}
// Update the task index for the next iteration
taskIndex++;
if (taskIndex > 3)
taskIndex = 1; // Reset to Task1 if we reach Task3
}
*/
int main(void){
OS_Init(); // initialize, disable interrupts, set PLL to 8 MHz
Timer0A_Init(8000000);
Timer1A_Init(8000000, Task1);
SYSCTL_RCGCGPIO_R |= 0x20; // activate clock for Port F
while((SYSCTL_PRGPIO_R&0x20) == 0){}; // allow time for clock to stabilize
GPIO_PORTF_DIR_R |= 0x0E; // make PF3-1 out
GPIO_PORTF_AFSEL_R &= ~0x0E; // disable alt funct on PF3-1
GPIO_PORTF_DEN_R |= 0x0E; // enable digital I/O on PF3-1
GPIO_PORTF_PCTL_R &= ~0x0000FFF0; // configure PF3-1 as GPIO
GPIO_PORTF_AMSEL_R &= ~0x0E; // disable analog functionality on PF3-1
sevenseg_run();
OS_AddThreads(&Task1, &Task2, &Task3);
OS_Launch(TIMESLICE); // doesn't return, interrupts enabled in here
return 0; // this never executes
}
