/** File Name		: joystick.h
*/

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

/* Includes --------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "piTankGoLib.h"

/* Defines --------------------------------------------------------*/
#define NUM_DIRECCIONES 5

/* Enums   --------------------------------------------------------*/
enum direccion_values {
	Y_POSITIVO,
	Y_NEGATIVO,
	X_POSITIVO,
	X_NEGATIVO,
	SW_ON,
};

/* Functions --------------------------------------------------------*/
void joystick_up_isr (void);
void joystick_down_isr (void);
void joystick_right_isr (void);
void joystick_left_isr (void);
void joystick_sw_isr (void);

int ConfiguraJoystick();


#endif /* JOYSTICK_H_ */

/**************************************************************************/
