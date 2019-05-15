/** File Name		: btn.h
*/

#ifndef BTN_H_
#define BTN_H_

/* Includes --------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "piTankGo_1.h"
#include "piTankGoLib.h"
#include "lcdpiTankGo.h"

/* Defines --------------------------------------------------------*/
#define NUM_BOTONES 3

/* Enums   --------------------------------------------------------*/
enum botones_values {
	BTN1,
	BTN2,
	BTN3,
};

/* Functions --------------------------------------------------------*/
void btn_1_isr (void);
void btn_2_isr (void);
void btn_3_isr (void);

int ConfiguraBTN(void);

#endif /* BTN_H_ */
