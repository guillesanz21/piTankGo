/** File Name		: piTankGoLib.h
 */

#ifndef _PITANKGOLIB_H_
#define _PITANKGOLIB_H_

/* Includes --------------------------------------------------------------*/
#include <stdio.h>

#include "fsm.h"
#include "tmr.h"

/* Defines --------------------------------------------------------------*/
#define CLK_MS 10

// INTERVALO DE GUARDA ANTI-REBOTES
#define	DEBOUNCE_TIME		100
#define DEBOUNCE_TIME_BTN	200
#define DEBOUNCE_TIME_KB	200
// DURACION DISPARO IR
#define SHOOTING_PERIOD 	1800
// DURACION CICLO US
#define CICLO_US		 	200

// CLAVES PARA MUTEX

#define	SYSTEM_FLAGS_KEY	1
#define	PLAYER_FLAGS_KEY	2
#define	STD_IO_BUFFER_KEY	3
#define	CONTROL_KEY			4

/* ---------------------------------------------------------------------
 * -----------------------------PINES GPIO------------------------------
 * ---------------------------------------------------------------------
 */

// Distribucion de pines GPIO empleada para el teclado

/*
#define	TECLADO_ROW_1	12
#define	TECLADO_ROW_2	16
#define	TECLADO_ROW_3	20
#define	TECLADO_ROW_4	21

#define	TECLADO_COL_1	5
#define	TECLADO_COL_2	6
#define	TECLADO_COL_3	19
#define	TECLADO_COL_4	26
*/

// Distribucion de pines GPIO empleada para el LCD

#define D7		26
#define D6		19
#define D5		6
#define D4		5

#define RS		11
#define E		10

// Distribucion de pines GPIO empleada para el joystick

#define JOY_PIN_UP		23
#define JOY_PIN_DOWN	24
#define JOY_PIN_LEFT	22
#define JOY_PIN_RIGHT	27
#define JOY_PIN_CENTER 	17

// Distribucion de pines GPIO empleada para el enlace IR

#define	IR_TX_PIN		12
#define	IR_RX_PIN		7

// Distribucion de pines GPIO empleada para la reproduccion de efectos

#define PLAYER_PWM_PIN 		4

// Distribucion de pines GPIO empleada para el control de los servos

#define	SERVO_VERTICAL_PIN		13
#define	SERVO_HORIZONTAL_PIN	18

// Distribucion de pines GPIO empleada para el control de los botones

#define	BTN_1		25
#define	BTN_2		16
#define	BTN_3		8

// Distribucion de pines GPIO empleada para el modulo de ultrasonidos

#define	US_TRIG		3
#define	US_ECHO		2

/* ---------------------------------------------------------------------
 * ------------------------------FLAGS----------------------------------
 * ---------------------------------------------------------------------
 */

// FLAGS FSM CONTROL DE JUEGO Y TORRETA

#define FLAG_SYSTEM_START_END 	0x001
#define FLAG_JOYSTICK_UP 		0x002
#define FLAG_JOYSTICK_DOWN 		0x004
#define FLAG_JOYSTICK_LEFT		0x008
#define FLAG_JOYSTICK_RIGHT		0x010
#define FLAG_TRIGGER_BUTTON		0x020
#define FLAG_SHOOT_TIMEOUT		0x040
#define FLAG_TARGET_DONE		0x080
#define FLAG_JUEGO_ON	 		0x100
//#define FLAG_SYSTEM_END			0x200

// FLAGS TECLADO

#define FLAG_KEY_PRESSED 		0x01
#define FLAG_TMR_TIMEOUT 		0x02


// FLAGS FSM REPRODUCCION DE EFECTOS DE SONIDO

#define FLAG_START_DISPARO 		0x01
#define FLAG_START_IMPACTO 		0x02
#define FLAG_PLAYER_STOP 		0x04
#define FLAG_PLAYER_END			0x08
#define FLAG_NOTA_TIMEOUT		0x10
#define FLAG_CAMBIO_C	 		0x20




/* Enum --------------------------------------------------------------*/
enum interruption_sources {
	TARGET_ISR,
	TECLADO_FILA_1,
	TECLADO_FILA_2,
	TECLADO_FILA_3,
	TECLADO_FILA_4,
	JOYSTICK_UP_ISR,
	JOYSTICK_DOWN_ISR,
	JOYSTICK_LEFT_ISR,
	JOYSTICK_RIGHT_ISR,
	JOYSTICK_CENTER_ISR
};


/* Variables --------------------------------------------------------*/
extern int flags_system; 	// Flag asociado al sistema
extern int flags_player;	// Flag asociado a player
extern int flags;			// Flag asociado al teclado matricial
extern int flags_juego;		// Flag asociado a la torreta
extern int juego_ON;		// Para saber cuando esta activo el juego


/*
 * Frecuencias y tiempos de las melodías de player.
 */
extern int frecuenciaDespacito[];
extern int tiempoDespacito[];
extern int frecuenciaGOT[];
extern int tiempoGOT[];
extern int frecuenciaTetris[];
extern int tiempoTetris[];
extern int frecuenciaStarwars[];
extern int tiempoStarwars[];

extern int frecuenciasDisparo[];
extern int tiemposDisparo[];
extern int frecuenciasImpacto[];
extern int tiemposImpacto[];

extern int impactos;			// Contador de impactos
extern int disparos;			// Contador de disparos
extern int cambioC;				// Indica a otro metodo que cancion debe sonar
extern int lcdControl2;			// LCD
extern double USdistance;		// Distancia medida por el ultrasonidos
extern char *strCancion;		// Indica que cancion esta sonando de fondo

#endif /* _PITANKGOLIB_H_ */


/**************************************************************************/
