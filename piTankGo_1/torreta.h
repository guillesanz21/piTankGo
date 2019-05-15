/** File Name		: torreta.h
 */

#ifndef _TORRETA_H_
#define _TORRETA_H_

/* Includes --------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "lcdpiTankGo.h"

#include "piTankGoLib.h"


/* Defines --------------------------------------------------------------*/
#define SERVO_PWM_RANGE		400 // 100 * 400 = 40,000 �S = 40 ms // 25 Hz

#define SERVO_INCREMENTO	1
#define SERVO_MINIMO		9
#define SERVO_MAXIMO		22


/* Structs   --------------------------------------------------------*/
typedef struct {
	int inicio; // Valor correspondiente a la posicion inicial del servo
	int incremento; // Cuantía en que se incrementa el valor de la posicion con cada movimiento del servo
	int minimo; // Valor mínimo correspondiente a la posicion del servo
	int maximo; // Valor maximo correspondiente a la posicion del servo
} TipoServo;

typedef struct {
	int x; // Coordenada x correspondiente a la posicion del servo horizontal
	int y; // Coordenada y correspondiente a la posicion del servo vertical
} TipoPosicionServo;

typedef struct {
	TipoPosicionServo posicion;
	TipoServo servo;
	tmr_t *tmr_torreta;

} TipoTorreta;


/* Functions --------------------------------------------------------*/
// Prototipos de procedimientos de inicializacion de los objetos especificos
void ConfiguraTorreta (TipoTorreta *p_torreta);
void InicializaTorreta (TipoTorreta *p_torreta);

// Prototipos de funciones de entrada
int CompruebaComienzo (fsm_t* this);
int CompruebaJoystickUp (fsm_t* this);
int CompruebaJoystickDown (fsm_t* fsm_player);
int CompruebaJoystickLeft (fsm_t* this);
int CompruebaJoystickRight (fsm_t* this);
int CompruebaTimeoutDisparo (fsm_t* this);
int CompruebaImpacto (fsm_t* this);
int CompruebaTriggerButton (fsm_t* this);
int CompruebaFinalJuego (fsm_t* this);
int CompruebaNoFlag (fsm_t* this);

// Prototipos de funciones de salida
void ComienzaSistema (fsm_t* this);
void MueveTorretaArriba (fsm_t* this);
void MueveTorretaAbajo (fsm_t* this);
void MueveTorretaIzquierda (fsm_t* this);
void MueveTorretaDerecha (fsm_t* this);
void DisparoIR (fsm_t* this);
void FinalDisparoIR (fsm_t* this);
void ImpactoDetectado (fsm_t* this);
void FinalizaJuego (fsm_t* this);
void ReiniciaJuego (fsm_t* this);

// Prototipos de procedimientos de atencion a las interrupciones
void timer_duracion_disparo_isr (union sigval value);

#endif /* _TORRETA_H_ */


/**************************************************************************/
