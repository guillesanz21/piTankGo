/** File Name		: keyboard.c
 *  Description		: Uso de teclado matricial para control del
 *  sistema y del juego. Se ha implementado con máquinas de
 *  estados y con interrupciones. Se excitan columnas para leer
 *  las filas.
 */


/* Includes --------------------------------------------------------------*/
#include "keyboard.h"

/* Private Variables -----------------------------------------------------*/
// Timeout to avoid bouncing after pin event
int debounceTime[NUM_ROWS] = {0,0,0,0};
char tecladoTL04[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
};


/* @Brief		Método encargado de activar el flag de tecla pulsada
 * 				cuando se pulse la fila 1. Además guardará en dos variables
 * 				que fila y columna se corresponden con la tecla pulsada.
 * @Note		La funcion usa la variable debounceTime para evitar rebotes
 */
void row_1_isr (void) {
    // Pin event (key / button event) debouncing procedure
    if (millis () < debounceTime[ROW_1]) {
        debounceTime[ROW_1] = millis () + DEBOUNCE_TIME_KB ;
        return;
    }

    piLock (CONTROL_KEY);
    teclado.teclaPulsada.row = ROW_1;
    teclado.teclaPulsada.col = teclado.columna_actual;
    flags |= FLAG_KEY_PRESSED;
    piUnlock (CONTROL_KEY);

    debounceTime[ROW_1] = millis () + DEBOUNCE_TIME_KB ;
}

/* @Brief		Método encargado de activar el flag de tecla pulsada
 * 				cuando se pulse la fila 2. Además guardará en dos variables
 * 				que fila y columna se corresponden con la tecla pulsada.
 * @Note		La funcion usa la variable debounceTime para evitar rebotes
 */
void row_2_isr (void) {
    // Pin event (key / button event) debouncing procedure
    if (millis () < debounceTime[ROW_2]) {
        debounceTime[ROW_2] = millis () + DEBOUNCE_TIME_KB ;
        return;
    }

    piLock (CONTROL_KEY);
    teclado.teclaPulsada.row = ROW_2;
    teclado.teclaPulsada.col = teclado.columna_actual;
    flags |= FLAG_KEY_PRESSED;

    piUnlock (CONTROL_KEY);

    debounceTime[ROW_2] = millis () + DEBOUNCE_TIME_KB ;
}

/* @Brief		Método encargado de activar el flag de tecla pulsada
 * 				cuando se pulse la fila 3. Además guardará en dos variables
 * 				que fila y columna se corresponden con la tecla pulsada.
 * @Note		La funcion usa la variable debounceTime para evitar rebotes
 */
void row_3_isr (void) {
    // Pin event (key / button event) debouncing procedure
    if (millis () < debounceTime[ROW_3]) {
        debounceTime[ROW_3] = millis () + DEBOUNCE_TIME_KB ;
        return;
    }

    piLock (CONTROL_KEY);
    teclado.teclaPulsada.row = ROW_3;
    teclado.teclaPulsada.col = teclado.columna_actual;
    flags |= FLAG_KEY_PRESSED;
    piUnlock (CONTROL_KEY);

    debounceTime[ROW_3] = millis () + DEBOUNCE_TIME_KB ;
}

/* @Brief		Método encargado de activar el flag de tecla pulsada
 * cuando se pulse la fila 4. Además guardará en dos variables que
 * fila y columna se corresponden con la tecla pulsada.
 * @Note		La funcion usa la variable debounceTime para evitar rebotes
 */
void row_4_isr (void) {
    // Pin event (key / button event) debouncing procedure
    if (millis () < debounceTime[ROW_4]) {
        debounceTime[ROW_4] = millis () + DEBOUNCE_TIME_KB ;
        return;
    }

    piLock (CONTROL_KEY);
    teclado.teclaPulsada.row = ROW_4;
    teclado.teclaPulsada.col = teclado.columna_actual;
    flags |= FLAG_KEY_PRESSED;
    piUnlock (CONTROL_KEY);

    debounceTime[ROW_4] = millis () + DEBOUNCE_TIME_KB ;
}


/* @Brief		Desactiva el flag asociado al temporizador
 * 				encargado de cambiar de columna.
 */
void timer_duracion_columna_isr (union sigval value) {
    piLock (CONTROL_KEY);
    flags |= FLAG_TMR_TIMEOUT;
    piUnlock (CONTROL_KEY);
}

/* @Brief		Configura los pines de la raspberry correspondientes
 * 				a las columnas y filas. Así como la asignacion del métodos
 * 				correspondientes con la activacion de las filas.
 * @Param	p_teclado		El objeto del teclado
 */
int ConfiguraTeclado(TipoTeclado *p_teclado){
    p_teclado->columna_actual = COL_1;

    // Inicialmente no hay tecla pulsada
    p_teclado->teclaPulsada.col = -1;
    p_teclado->teclaPulsada.row = -1;

    /*
    // Configuración e interrupción de los pines de las filas
    pinMode (TECLADO_ROW_1, INPUT);
    pullUpDnControl(TECLADO_ROW_1, PUD_DOWN);
    wiringPiISR (TECLADO_ROW_1, INT_EDGE_RISING, row_1_isr);

    pinMode (TECLADO_ROW_2, INPUT);
    pullUpDnControl(TECLADO_ROW_2, PUD_DOWN);
    wiringPiISR (TECLADO_ROW_2, INT_EDGE_RISING, row_2_isr);

    pinMode (TECLADO_ROW_3, INPUT);
    pullUpDnControl(TECLADO_ROW_3, PUD_DOWN);
    wiringPiISR (TECLADO_ROW_3, INT_EDGE_RISING, row_3_isr);

    pinMode (TECLADO_ROW_4, INPUT);
    pullUpDnControl(TECLADO_ROW_4, PUD_DOWN);
    wiringPiISR (TECLADO_ROW_4, INT_EDGE_RISING, row_4_isr);

	// Configuracion de los pines de las columnas
    pinMode (TECLADO_COL_1, OUTPUT);
    digitalWrite (TECLADO_COL_1, HIGH);

    pinMode (TECLADO_COL_2, OUTPUT);
    digitalWrite (TECLADO_COL_2, LOW);

    pinMode (TECLADO_COL_3, OUTPUT);
    digitalWrite (TECLADO_COL_3, LOW);

    pinMode (TECLADO_COL_4, OUTPUT);
    digitalWrite (TECLADO_COL_4, LOW);
	*/

    /* Creación e inicio del temporizador */
    p_teclado->tmr_duracion_columna = tmr_new (timer_duracion_columna_isr);
    tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna), COL_REFRESH_TIME);

    printf("\nSystem init complete! keypad ready to process the code!!!\n");
    fflush(stdout);
    return 0;
}

/* @Brief		Comprueba si el temporizador ha terminado
 * @Param	this	Asociado al objeto TipoTeclado
 * @Return		Devuelve 1 si ha terminado, 0 si sigue.
 */
int CompruebaColumnTimeout (fsm_t* this) {
	int result = 0;
	piLock (CONTROL_KEY);
	result = (flags & FLAG_TMR_TIMEOUT);
	piUnlock (CONTROL_KEY);
	return result;
}

/* @Brief		Excita la columna 1 y guarda que columna es la actual
 * @Param	this	Asociado al objeto TipoTeclado
 * @Note		Inicia el temporizador.
 */
void col_1 (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	piLock (CONTROL_KEY);
/*
	digitalWrite (TECLADO_COL_4, LOW);
	digitalWrite (TECLADO_COL_1, HIGH);
	digitalWrite (TECLADO_COL_2, LOW);
	digitalWrite (TECLADO_COL_3, LOW);
*/
	flags &= (~FLAG_TMR_TIMEOUT);

	p_teclado->columna_actual = COL_1;

	piUnlock (CONTROL_KEY);

	tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna), COL_REFRESH_TIME);
}

/* @Brief		Excita la columna 2 y guarda que columna es la actual
 * @Param	this	Asociado al objeto TipoTeclado
 * @Note		Inicia el temporizador.
 */
void col_2 (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	piLock (CONTROL_KEY);
/*
	digitalWrite (TECLADO_COL_1, LOW);
	digitalWrite (TECLADO_COL_2, HIGH);
	digitalWrite (TECLADO_COL_3, LOW);
	digitalWrite (TECLADO_COL_4, LOW);
*/
	flags &= (~FLAG_TMR_TIMEOUT);

	p_teclado->columna_actual = COL_2;

	piUnlock (CONTROL_KEY);

	tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna), COL_REFRESH_TIME);
}

/* @Brief		Excita la columna 3 y guarda que columna es la actual
 * @Param	this	Asociado al objeto TipoTeclado
 * @Note		Inicia el temporizador.
 */
void col_3 (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	piLock (CONTROL_KEY);
/*
	digitalWrite (TECLADO_COL_2, LOW);
	digitalWrite (TECLADO_COL_3, HIGH);
	digitalWrite (TECLADO_COL_4, LOW);
	digitalWrite (TECLADO_COL_1, LOW);
*/
	flags &= (~FLAG_TMR_TIMEOUT);

	p_teclado->columna_actual = COL_3;

	piUnlock (CONTROL_KEY);

	tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna), COL_REFRESH_TIME);
}

/* @Brief		Excita la columna 4 y guarda que columna es la actual
 * @Param	this	Asociado al objeto TipoTeclado
 * @Note		Inicia el temporizador.
 */
void col_4 (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	piLock (CONTROL_KEY);
/*
	digitalWrite (TECLADO_COL_3, LOW);
	digitalWrite (TECLADO_COL_4, HIGH);
	digitalWrite (TECLADO_COL_1, LOW);
	digitalWrite (TECLADO_COL_2, LOW);
*/
	flags &= (~FLAG_TMR_TIMEOUT);

	p_teclado->columna_actual = COL_4;

	piUnlock (CONTROL_KEY);

	tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna), COL_REFRESH_TIME);
}


/* @Brief		Comprueba si se ha pulsado una tecla
 * @Param	this	Asociado al objeto TipoTeclado
 * @Return		Devuelve 1 si se ha pulsado, 0 si no.
 */
int key_pressed (fsm_t* this) {
	int result = 0;

	piLock (CONTROL_KEY);
	result = (flags & FLAG_KEY_PRESSED);
	piUnlock (CONTROL_KEY);

	return result;
}

/* @Brief		Comprueba que tecla se ha pulsado, dependiendo de cual,
 * 				hace una cosa u otra.
 * @Param	this	Asociado al objeto TipoTeclado
 * @Note		Desactiva el flag FLAG_KEY_PRESSED.
 */
void process_key (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	piLock (CONTROL_KEY);

	flags &= (~FLAG_KEY_PRESSED);

	printf("Fila: %d, Columna: %d\n",p_teclado->teclaPulsada.row, p_teclado->teclaPulsada.col);

	switch(p_teclado->teclaPulsada.col){
		case COL_1:
		case COL_2:
		case COL_3:
		case COL_4:
			printf("\nKeypress \"%c\"...\n", tecladoTL04[p_teclado->teclaPulsada.row][p_teclado->teclaPulsada.col]);
			// Si se pulsa '2'
			if(tecladoTL04[p_teclado->teclaPulsada.row][p_teclado->teclaPulsada.col] == tecladoTL04[0][1]){
				piLock(SYSTEM_FLAGS_KEY);
				flags_juego |= FLAG_JOYSTICK_UP;
				piUnlock(SYSTEM_FLAGS_KEY);
				piLock (STD_IO_BUFFER_KEY);
				printf("Coordenada Y positiva!\n");
				piUnlock (STD_IO_BUFFER_KEY);
			}
			// Si se pulsa '8'
			else if(tecladoTL04[p_teclado->teclaPulsada.row][p_teclado->teclaPulsada.col] == tecladoTL04[2][1]){
				piLock(SYSTEM_FLAGS_KEY);
				flags_juego |= FLAG_JOYSTICK_DOWN;
				piUnlock(SYSTEM_FLAGS_KEY);
				piLock (STD_IO_BUFFER_KEY);
				printf("Coordenada Y negativa!\n");
				piUnlock (STD_IO_BUFFER_KEY);
			}
			// Si se pulsa '4'
			else if(tecladoTL04[p_teclado->teclaPulsada.row][p_teclado->teclaPulsada.col] == tecladoTL04[1][0]){
				piLock(SYSTEM_FLAGS_KEY);
				flags_juego |= FLAG_JOYSTICK_LEFT;
				piUnlock(SYSTEM_FLAGS_KEY);
				piLock (STD_IO_BUFFER_KEY);
				printf("Coordenada X negativa!\n");
				piUnlock (STD_IO_BUFFER_KEY);
			}
			// Si se pulsa '6'
			else if(tecladoTL04[p_teclado->teclaPulsada.row][p_teclado->teclaPulsada.col] == tecladoTL04[1][2]){
				piLock(SYSTEM_FLAGS_KEY);
				flags_juego |= FLAG_JOYSTICK_RIGHT;
				piUnlock(SYSTEM_FLAGS_KEY);
				piLock (STD_IO_BUFFER_KEY);
				printf("Coordenada X positiva!\n");
				piUnlock (STD_IO_BUFFER_KEY);
			}
			// Si se pulsa 'A'
			else if (tecladoTL04[p_teclado->teclaPulsada.row][p_teclado->teclaPulsada.col] == tecladoTL04[0][3]){
				piLock (SYSTEM_FLAGS_KEY);
				flags_juego |= FLAG_SYSTEM_START_END;
				piUnlock (SYSTEM_FLAGS_KEY);
			}
			// Si se pulsa '5'
			else if(tecladoTL04[p_teclado->teclaPulsada.row][p_teclado->teclaPulsada.col] == tecladoTL04[1][1]){
				if (juego_ON == 1) {
		    		piLock (SYSTEM_FLAGS_KEY);
		    		flags_juego |= FLAG_TRIGGER_BUTTON;
		        	piUnlock (SYSTEM_FLAGS_KEY);
				} else {
					printf ("Juego acabado: No puedes disparar!");
				}
			}
			// Si se pulsa 'B' o '1'
			else if (tecladoTL04[p_teclado->teclaPulsada.row][p_teclado->teclaPulsada.col] == tecladoTL04[1][3] ||
					tecladoTL04[p_teclado->teclaPulsada.row][p_teclado->teclaPulsada.col] == tecladoTL04[0][0] ){

				if (juego_ON == 1) {
					piLock(SYSTEM_FLAGS_KEY);
					flags_juego |= FLAG_TARGET_DONE;
					piUnlock(SYSTEM_FLAGS_KEY);
				} else {
					printf ("Juego acabado: No se puede impactar!");
				}
			}
			// si se pulsa '*'
			else if (tecladoTL04[p_teclado->teclaPulsada.row][p_teclado->teclaPulsada.col] == tecladoTL04[3][0]){
				exit(0);
			} else {
				piLock (STD_IO_BUFFER_KEY);
				printf("La tecla pulsada no tiene ninguna función asignada.\n");
				piUnlock (STD_IO_BUFFER_KEY);
			}

			fflush(stdout);
			break;

		default:
			printf("\nERROR!!!! invalid number of column (%d)!!!\n", p_teclado->teclaPulsada.col);
			fflush(stdout);

			p_teclado->teclaPulsada.row = -1;
			p_teclado->teclaPulsada.col = -1;

			break;
	}
	piUnlock (CONTROL_KEY);
}


/**************************************************************************/
