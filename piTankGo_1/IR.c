/** File Name		: IR.c
 *  Description		: Comprueba las entradas correspondientes al receptor IR
 *  y activa los correspondientes flags de impacto.
 */

/* Includes --------------------------------------------------------*/
#include "IR.h"


/* @Brief		Método encargado de activar el flag correspondiente al impacto
 * cuando el juego esté activo.
 */
void rx_IR_isr (void) {

	if (juego_ON == 1) {
		piLock(SYSTEM_FLAGS_KEY);
		flags_juego |= FLAG_TARGET_DONE;
		piUnlock(SYSTEM_FLAGS_KEY);
	} else {
		printf ("Juego acabado: No hay impactos!");
	}
}

/* @Brief		Método encargado de configurar los pines de la raspberry
 * correspondientes a los LED's emisor y receptor IR. Así como la asignacion
 * del metodo correspondiente con la activacion del IR receptor,
 * activacion por flanco de bajada.
 */
int ConfiguraIR(void) {

    /* Configuración del pin para IR transmisor*/
    pinMode (IR_TX_PIN, OUTPUT);
    digitalWrite (IR_TX_PIN, LOW);

    /* Configuración e interrupción del pin para IR receptor*/
    pinMode (IR_RX_PIN, INPUT);
    pullUpDnControl(IR_RX_PIN, PUD_DOWN);
    wiringPiISR (IR_RX_PIN, INT_EDGE_FALLING, rx_IR_isr);

    return 0;

}

/**************************************************************************/
