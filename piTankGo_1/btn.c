/** File Name		: btn.c
 *  Description		: Mide las entradas correspondientes los botones,
 *  activando los flags correspondientes al inicio y final del sistema.
 */

/* Includes --------------------------------------------------------------*/
#include "btn.h"

/* Private Variables -----------------------------------------------------*/
int debounceTimeBotones[NUM_BOTONES] = {0,0}; // Anti rebotes


/* @Brief		Método encargado de activar el flag correspondiente al inicio
 * del sistema, asociado al boton 1
 * @Note		La funcion usa debounceTimeBotones para evitar rebotes
 */
void btn_1_isr (void) {
    // Pin event (key / button event) debouncing procedure
    if (millis () < debounceTimeBotones[BTN1]) {
    	debounceTimeBotones[BTN1] = millis () + DEBOUNCE_TIME_BTN ;
        return;
    }

    /* piLock (STD_IO_BUFFER_KEY);
	printf("Botón 1 pulsado!\n");
	piUnlock (STD_IO_BUFFER_KEY); */

	piLock (SYSTEM_FLAGS_KEY);
	flags_juego |= FLAG_SYSTEM_START_END;
	piUnlock (SYSTEM_FLAGS_KEY);

	debounceTimeBotones[BTN1] = millis () + DEBOUNCE_TIME_BTN ;
}


/* @Brief		Método encargado de cambiar la variable de cambio de cancion
 * @Note		La funcion usa debounceTimeBotones para evitar rebotes
 */
void btn_2_isr (void) {
    // Pin event (key / button event) debouncing procedure
    if (millis () < debounceTimeBotones[BTN2]) {
    	debounceTimeBotones[BTN2] = millis () + DEBOUNCE_TIME_BTN ;
        return;
    }

    piLock(PLAYER_FLAGS_KEY);
	flags_player |= FLAG_CAMBIO_C;
	piUnlock(PLAYER_FLAGS_KEY);

    debounceTimeBotones[BTN2] = millis () + DEBOUNCE_TIME_BTN ;
}

/* @Brief		Método encargado de activar el flag correspondiente al final
 * del programa, asociado al boton 3
 * @Note		La funcion usa debounceTimeBotones para evitar rebotes
 */
void btn_3_isr (void) {
    // Pin event (key / button event) debouncing procedure
    if (millis () < debounceTimeBotones[BTN3]) {
    	debounceTimeBotones[BTN3] = millis () + DEBOUNCE_TIME_BTN ;
        return;
    }

	piLock (STD_IO_BUFFER_KEY);
	printf("Salida del sistema!\n");
	piUnlock (STD_IO_BUFFER_KEY);

	lcdClear(lcdControl2);
	lcd(2,lcdControl2);

	exit(0);

	debounceTimeBotones[BTN3] = millis () + DEBOUNCE_TIME_BTN ;
}

/* @Brief		Método encargado de inicializar y configurar los pines de la RPi
 * asociados a los botones, así como asignar el método correspondiente con la
 * activacion de cada uno de los botones. Las dos activaciones son por flanco de subida.
 */
int ConfiguraBTN(void){

    /* Configuración e interrupción del pin para el boton 1*/
    pinMode(BTN_1, INPUT);
    pullUpDnControl(BTN_1, PUD_DOWN);
    wiringPiISR (BTN_1, INT_EDGE_RISING, btn_1_isr);

    /* Configuración e interrupción del pin para el boton 2*/
    pinMode(BTN_2, INPUT);
    pullUpDnControl(BTN_2, PUD_DOWN);
    wiringPiISR (BTN_2, INT_EDGE_RISING, btn_2_isr);

    /* Configuración e interrupción del pin para el boton 3*/
    pinMode(BTN_3, INPUT);
    pullUpDnControl(BTN_3, PUD_DOWN);
    wiringPiISR (BTN_3, INT_EDGE_RISING, btn_3_isr);

    return 0;
}
