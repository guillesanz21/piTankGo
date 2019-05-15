/** File Name		: IR.h
*/

#ifndef IR_H_
#define IR_H_

/* Includes --------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "piTankGoLib.h"

/* Functions --------------------------------------------------------*/
void rx_IR_isr (void);
int ConfiguraIR(void);

#endif /* IR_H_ */

/**************************************************************************/
