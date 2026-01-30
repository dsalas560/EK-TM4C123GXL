#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "Timer0A.h"
#include "SSI2.h"
#include "LCD.h"
#include "Timer2A.h"
extern uint32_t period;
uint16_t rpm;
uint32_t rps = 0;
//void Timer0A_Wait1ms(int n);
//volatile uint32_t period;
void RPM(){
/*
frequency = 1.0/period;
pulses = frequency * 1000;
rps = pulses/960;
rpm = rps *60;
*/
rps = (60 * 8000000) /960;
rpm = rps/period;
// rpm = trial *60;
return;
}
int main(void) {
int pw = 0;
PLL_Init(Bus8MHz); // set system clock to 8 MHz
Timer0A_Init(8000000); // initialize Timer0A for 8 MHz clock
SSI2_init(); // initialize SSI2 module for SPI
LCD_init(); // initialize LCD controller
Timer2A_Init();
RPM();
//Timer2A_Handler();
//trial = 60 * Bus8MHz;
SYSCTL_RCGCPWM_R |= 0x02; // enable clock to PWM1
SYSCTL_RCGCGPIO_R |= 0x20; // enable clock to GPIOF
SYSCTL_RCGCGPIO_R |= 0x02; // enable clock to GPIOB
Timer0A_Wait1ms(1); // PWM1 seems to take a while to start
SYSCTL_RCC_R &= ~0x00100000; // use system clock for PWM
PWM1_INVERT_R |= 0x80; // positive pulse
PWM1_3_CTL_R = 0; // disable PWM1_3 during configuration
PWM1_3_GENB_R = 0x0000080C; // output high when load and low when match
PWM1_3_LOAD_R = 3999; // 4 kHz
PWM1_3_CTL_R = 1; // enable PWM1_3
PWM1_ENABLE_R |= 0x80; // enable PWM1 only one that didn't have _ before
GPIO_PORTF_DIR_R |= 0x08; // set PORTF 3 pins as output (LED) pin
GPIO_PORTF_DEN_R |= 0x08; // set PORTF 3 pins as digital pins
GPIO_PORTF_AFSEL_R |= 0x08; // enable alternate function
GPIO_PORTF_PCTL_R &= ~0x0000F000; // clear PORTF 3 alternate function
GPIO_PORTF_PCTL_R |= 0x00005000; // set PORTF 3 alternate funtion to PWM
GPIO_PORTB_DEN_R |= 0x0C; // PORTB 3 as digital pins
GPIO_PORTB_DIR_R |= 0x0C; // set PORTB 3 as output
GPIO_PORTB_DATA_R |= 0x08; // enable PORTB 3
// Timer2A_Init();
while(1) {
// set direction
GPIO_PORTB_DATA_R &= ~0x04;
GPIO_PORTB_DATA_R |= 0x08;
// ramp up speed
for (pw = 100; pw < 3999; pw += 20) {
PWM1_3_CMPB_R = pw;
Timer0A_Wait1ms(250);
LCD_Clear();
// LCD_OutUDec(period);
RPM();
LCD_OutUFix(rpm);
}
// ramp down speed
for (pw = 3900; pw >100; pw -= 20) {
PWM1_3_CMPB_R = pw;
Timer0A_Wait1ms(250);
LCD_Clear();
// LCD_OutUDec(period);
RPM();
LCD_OutUFix(rpm);
}
// reverse direction
GPIO_PORTB_DATA_R &= ~0x08;
GPIO_PORTB_DATA_R |= 0x04;
// ramp up speed
for (pw = 100; pw < 3999; pw += 20) {
PWM1_3_CMPB_R = pw;
Timer0A_Wait1ms(250);
LCD_Clear();
//LCD_OutUDec(period);
RPM();
LCD_OutUFix(rpm);
}
// ramp down speed
for (pw = 3900; pw >100; pw -= 20) {
PWM1_3_CMPB_R = pw;
Timer0A_Wait1ms(250);
LCD_Clear();
//LCD_OutUDec(period);
RPM();
LCD_OutUFix(rpm);
}
}
}
