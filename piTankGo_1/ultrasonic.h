/** File Name		: ultrasonic.h
*/
#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

/* Includes --------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "piTankGoLib.h"
#include "lcdpiTankGo.h"

/* Defines --------------------------------------------------------*/
#define DISTANCE_SHOOT 7.0

/* Struct ---------------------------------------------------------*/
typedef struct {
	unsigned int t_inicio;
	unsigned int t_final;
	double distance;

} TipoUltraSonido;

/* Variables --------------------------------------------------------*/
extern TipoUltraSonido ultrasonido;

/* Functions --------------------------------------------------------*/
PI_THREAD(thread_ultrasonidos);

int ConfiguraUS();

#endif /* ULTRASONIC_H_ */

/**************************************************************************/
