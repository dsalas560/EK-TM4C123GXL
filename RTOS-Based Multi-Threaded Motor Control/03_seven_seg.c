/* Seven-segment display counter
*
* This program counts number 0 -3 on the seven segment display.
* The seven segment display is driven by a shift register which is
* connected to SSI2 in SPI mode.
*
* Built and tested with Keil MDK-ARM v5.28 and TM4C_DFP v1.1.0
*/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "SSI2.h"
#include "Timer0A.h"
#include "Timer1A.h"
int i=0;
void sevenseg_init(void);
void SSI2_write_sevenseg(unsigned char data);
void sevenseg_run(void);
void sevenseg_run(void) {
sevenseg_init(); // initialize SSI2 that connects to the shift registers
Timer1A_Init(8000000, Timer1A_Handler);
}
// enable SSI2 and associated GPIO pins
void sevenseg_init(void) {
SYSCTL_RCGCGPIO_R |= 0x02; // enable clock to GPIOB
SYSCTL_RCGCGPIO_R |= 0x04; // enable clock to GPIOC
SYSCTL_RCGCSSI_R |= 0x04; // enable clock to SSI2
// PORTB 7, 4 for SSI2 TX and SCLK
GPIO_PORTB_AMSEL_R &= ~0x90; // turn off analog of PORTB 7, 4
GPIO_PORTB_AFSEL_R |= 0x90; // PORTB 7, 4 for alternate function
GPIO_PORTB_PCTL_R &= ~0xF00F0000; // clear functions for PORTB 7, 4
GPIO_PORTB_PCTL_R |= 0x20020000; // PORTB 7, 4 for SSI2 function
GPIO_PORTB_DEN_R |= 0x90; // PORTB 7, 4 as digital pins
// PORTC 7 for SSI2 slave select
GPIO_PORTC_AMSEL_R &= ~0x80; // disable analog of PORTC 7
GPIO_PORTC_DATA_R |= 0x80; // set PORTC 7 idle high
GPIO_PORTC_DIR_R |= 0x80; // set PORTC 7 as output for SS
GPIO_PORTC_DEN_R |= 0x80; // set PORTC 7 as digital pin
SSI2_CR1_R = 0; // turn off SSI2 during configuration
SSI2_CC_R = 0; // use system clock
SSI2_CPSR_R = 16; // clock prescaler divide by 16 gets 1 MHz clock
SSI2_CR0_R = 0x0007; // clock rate div by 1, phase/polarity 0 0, mode
freescale, data size 8
SSI2_CR1_R = 2; // enable SSI2 as master
}
void SSI2_write_sevenseg(unsigned char data) {
GPIO_PORTC_DATA_R &= ~0x80; // assert slave select
SSI2_DR_R = data; // write data
while (SSI2_SR_R & 0x10) {} // wait for transmit done
GPIO_PORTC_DATA_R |= 0x80; // deassert slave select
}
