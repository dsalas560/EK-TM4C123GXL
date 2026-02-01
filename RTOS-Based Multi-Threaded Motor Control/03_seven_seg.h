//#endif /* 03_SEVEN_SEG_H_ */+
/*
* 03_seven_seg.h
*
* Created on: Mar 20, 2024
* Author: daniel
*/
#ifndef SEVENSEGDISPLAY_H_
#define SEVENSEGDISPLAY_H_
/**
* @brief Initialize the seven-segment display
*/
void sevenseg_init(void);
/**
* @brief Run the seven-segment display counter
*/
void sevenseg_run(void);
void SSI2_write_sevenseg(unsigned char data);
#endif /* SEVENSEGDISPLAY_H_ */
