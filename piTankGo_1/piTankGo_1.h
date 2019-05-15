/** File Name		: piTankGo_1.h
 */

#ifndef _PITANKGO_1_H_
#define _PITANKGO_1_H_

/* Includes -----------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#include "kbhit.h"

#include "player.h"
#include "torreta.h"
#include "IR.h"
#include "btn.h"
#include "keyboard.h"
#include "joystick.h"
#include "ultrasonic.h"
#include "lcdpiTankGo.h"


/* Enum ----------------------------------------------------------------*/
// Posibles estados de las FSMs
enum fsm_state {
	WAIT_START,
	WAIT_KEY,
	WAIT_PUSH,
	WAIT_NEXT,
	WAIT_MOVE,
	WAIT_END,
	JOYSTICK_UP,
	JOYSTICK_DOWN,
	JOYSTICK_LEFT,
	JOYSTICK_RIGHT,
	TRIGGER_BUTTON,
};

/* Struct --------------------------------------------------------------*/
typedef struct {
	TipoTorreta torreta; // Objeto para el control de la torreta
	TipoPlayer player; // Reproductor de efectos
	char teclaPulsada;  // Variable que almacena la ultima tecla pulsada
	int debug; // Variable que habilita o deshabilita la impresion de mensajes por salida estandar
} TipoSistema;

/* Variables -------------------------------------------------------------*/
TipoTeclado teclado;
TipoUltraSonido ultrasonido;

//------------------------------------------------------
// FUNCIONES DE CONFIGURACION/INICIALIZACION
//------------------------------------------------------
int ConfiguraSistema (TipoSistema *p_sistema);
int InicializaSistema (TipoSistema *p_sistema);

//------------------------------------------------------
// FUNCIONES LIGADAS A THREADS ADICIONALES
//------------------------------------------------------
PI_THREAD(thread_explora_teclado_PC);

#endif /* _PITANKGO_1_H_ */


/**************************************************************************/
