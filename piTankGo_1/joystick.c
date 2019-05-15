/** File Name		: joystick.c
 *  Description		: Mide los pines de entrada conectados al Arduino,
 *  de tal modo que cuando se detecte un movimiento en el joystick,
 *  se active el flag correspondiente al movimiento de la torreta(torreta.h).
 */

/* Includes -----------------------------------------------------------------*/
#include "joystick.h"

/* Private Variables --------------------------------------------------------*/
int debounceTimeJoystick[NUM_DIRECCIONES] = {0,0,0,0,0}; // Anti rebotes
int flag_TOC; //Flag que se usa para evitar la ejecucion de dos movimientos de la torreta
			  // simultaneamente, al ser un joystick analogico y con la capacidad de
			  //rotar 360º, se pueden activar erróneamente dos movimientos de la torreta.


/* @Brief		Método encargado de activar el flag correspondiente al movimiento
 *  			positivo en el eje Y de la torreta
 * @Note		La funcion usa la variable debounceTimeJoystick para evitar rebotes
 */
void joystick_up_isr (void) {
    // Pin event (key / button event) debouncing procedure

	if (millis () < debounceTimeJoystick[Y_POSITIVO]) {
	        debounceTimeJoystick[Y_POSITIVO] = millis () + DEBOUNCE_TIME ;
	        return;
	}
	if(flag_TOC) {

		flag_TOC = 0;
		piLock (SYSTEM_FLAGS_KEY);
		flags_juego |= FLAG_JOYSTICK_UP;
		piUnlock (SYSTEM_FLAGS_KEY);
		piLock (STD_IO_BUFFER_KEY);
		piUnlock (STD_IO_BUFFER_KEY);
		flag_TOC = 1;
	}
    debounceTimeJoystick[Y_POSITIVO] = millis () + DEBOUNCE_TIME ;
}

/* @Brief		Método encargado de activar el flag correspondiente al movimiento
 *  			negativo en el eje Y de la torreta
 */
void joystick_down_isr (void) {
    // Pin event (key / button event) debouncing procedure

	if (millis () < debounceTimeJoystick[Y_NEGATIVO]) {
	        debounceTimeJoystick[Y_NEGATIVO] = millis () + DEBOUNCE_TIME ;
	        return;
	}
	if(flag_TOC) {

		flag_TOC = 0;
		piLock (STD_IO_BUFFER_KEY);
		piLock (SYSTEM_FLAGS_KEY);
		flags_juego |= FLAG_JOYSTICK_DOWN;
		piUnlock (SYSTEM_FLAGS_KEY);
		piUnlock (STD_IO_BUFFER_KEY);
		flag_TOC = 1;
	}
    debounceTimeJoystick[Y_NEGATIVO] = millis () + DEBOUNCE_TIME ;
}

/* @Brief		Método encargado de activar el flag correspondiente al movimiento
 *  			positivo en el eje X de la torreta
 * @Note		La funcion usa la variable debounceTimeJoystick para evitar rebotes
 */
void joystick_right_isr (void) {
    // Pin event (key / button event) debouncing procedure

	if (millis () < debounceTimeJoystick[X_POSITIVO]) {
	        debounceTimeJoystick[X_POSITIVO] = millis () + DEBOUNCE_TIME ;
	        return;
	}
	if(flag_TOC) {

		flag_TOC = 0;
		piLock (SYSTEM_FLAGS_KEY);
		piLock (STD_IO_BUFFER_KEY);
		flags_juego |= FLAG_JOYSTICK_LEFT;
		piUnlock (SYSTEM_FLAGS_KEY);
		piUnlock (STD_IO_BUFFER_KEY);
		flag_TOC = 1;
	}
    debounceTimeJoystick[X_POSITIVO] = millis () + DEBOUNCE_TIME ;
}

/* @Brief		Método encargado de activar el flag correspondiente al movimiento
 *  			negativo en el eje X de la torreta
 * @Note		La funcion usa la variable debounceTimeJoystick para evitar rebotes
 */
void joystick_left_isr (void) {
    // Pin event (key / button event) debouncing procedure

	if (millis () < debounceTimeJoystick[X_NEGATIVO]) {
	        debounceTimeJoystick[X_NEGATIVO] = millis () + DEBOUNCE_TIME ;
	        return;
	}
	if(flag_TOC) {
		flag_TOC = 0;
    	piLock (SYSTEM_FLAGS_KEY);
    	piLock (STD_IO_BUFFER_KEY);
    	flags_juego |= FLAG_JOYSTICK_RIGHT;
    	piUnlock (SYSTEM_FLAGS_KEY);
    	piUnlock (STD_IO_BUFFER_KEY);
		flag_TOC = 1;
	}
    debounceTimeJoystick[X_NEGATIVO] = millis () + DEBOUNCE_TIME ;
}

/* @Brief		Método encargado de activar el flag correspondiente botón de disparo
 * de la torreta, el disparo se ejecutará en "torreta.c" con el correspondiente flag
 * que llamará a la funcion de "player.c". Esta función unicamente activa el flag de
 * boton pulsado, "torreta.c" se encarga de el resto del proceso.
 * @Note		La funcion usa la variable debounceTimeJoystick para evitar rebotes
 */
void joystick_sw_isr (void) {
    // Pin event (key / button event) debouncing procedure
    if (millis () < debounceTimeJoystick[SW_ON]) {
        debounceTimeJoystick[SW_ON] = millis () + DEBOUNCE_TIME ;
        return;
    }
	if(flag_TOC) {

		flag_TOC = 0;
    	piLock (STD_IO_BUFFER_KEY);
    	if (juego_ON == 1) {
    		piLock (SYSTEM_FLAGS_KEY);
    		flags_juego |= FLAG_TRIGGER_BUTTON;
    		piUnlock (SYSTEM_FLAGS_KEY);
    	} else {
    		printf ("Juego acabado: No puedes disparar!");
    	}
		flag_TOC = 1;
    }
    piUnlock (STD_IO_BUFFER_KEY);
    debounceTimeJoystick[SW_ON] = millis () + DEBOUNCE_TIME ;
}


/* @Brief		Método encargado iniciar todas las variables e inicializar los pines
 * de la raspberry con su correspondiente funcion asociada a la interrupcion provocada
 * por la activacion de dicho pin, la activacion se hace por flanco de subida en todos
 * los pines.
 */
int ConfiguraJoystick(void){

    /* Inicialización de parámetros */
    flag_TOC = 1;

    /* Configuración e interrupción del pin para Joystick UP*/
    pinMode (JOY_PIN_UP, INPUT);
    pullUpDnControl(JOY_PIN_UP, PUD_DOWN);
    wiringPiISR (JOY_PIN_UP, INT_EDGE_RISING, joystick_up_isr);

    /* Configuración e interrupción del pin para Joystick DOWN*/
    pinMode (JOY_PIN_DOWN, INPUT);
    pullUpDnControl(JOY_PIN_DOWN, PUD_DOWN);
    wiringPiISR (JOY_PIN_DOWN, INT_EDGE_RISING, joystick_down_isr);

    /* Configuración e interrupción del pin para Joystick RIGHT*/
    pinMode (JOY_PIN_RIGHT, INPUT);
    pullUpDnControl(JOY_PIN_RIGHT, PUD_DOWN);
    wiringPiISR (JOY_PIN_RIGHT, INT_EDGE_RISING, joystick_right_isr);

    /* Configuración e interrupción del pin para Joystick LEFT*/
    pinMode (JOY_PIN_LEFT, INPUT);
    pullUpDnControl(JOY_PIN_LEFT, PUD_DOWN);
    wiringPiISR (JOY_PIN_LEFT, INT_EDGE_RISING, joystick_left_isr);

    /* Configuración e interrupción del pin para Joystick SW*/
    pinMode (JOY_PIN_CENTER, INPUT);
    pullUpDnControl(JOY_PIN_CENTER, PUD_DOWN);
    wiringPiISR (JOY_PIN_CENTER, INT_EDGE_RISING, joystick_sw_isr);


    return 0;
}


/**************************************************************************/
