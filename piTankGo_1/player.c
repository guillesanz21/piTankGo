/** File Name		: player.c
 *  Description		: Inicializa los efectos de sonido de impacto y disparo,
 *  y reproduce esos efectos cuando se activan los FLAGS correspondientes.
 *  Esos flags se activan en otros ficheros tales como joystick o ultrasonic.
 */

/* Includes -----------------------------------------------------------------*/
#include "player.h"

/* Private Variables --------------------------------------------------------*/
int i;		// Variable que itera el bucle del metodo InicializaEfecto
int fondo;	// Para saber que efecto esta reproduciendose

/* @Brief		Se inicializan los valores del Efecto. Los valores se
 * 				copian de los pasados al método.
 * @param	*p_efecto	Es el puntero de la estructura vacia a rellenar.
 * @param	*nombre		El nombre de la estructura.
 * @param	*array_frecuencias	Las frecuencias del efecto.
 * @param	*array_duraciones	Las duraciones del efecto.
 * @param	num_notas	El numero de notas del efecto.
 * @return		Devuelve el numero de notas del efecto
 */
int InicializaEfecto (TipoEfecto *p_efecto, char *nombre, int *array_frecuencias, int *array_duraciones, int num_notas) {

	i = 0;			// Iterador
	strcpy(p_efecto->nombre, nombre);		/* El nombre es un array de char */
	p_efecto->num_notas = 0;
	/* Rellenamos los arrays de frecuencias y duraciones */
	for (i = 0; i < num_notas; i++){
		p_efecto->frecuencias[i] = array_frecuencias[i];
		p_efecto->duraciones[i] = array_duraciones[i];
		p_efecto->num_notas++;
	}
	return p_efecto->num_notas;
}

/* @Brief		Se inicializan los valores del Player. Se inicializan 3 efectos,
 * 				el de disparo, el de impacto y el de la musica de fondo. Se apunta
 * 				inicialmente al efecto de fondo, y a lo largo del programa, ira
 * 				apuntando a los otros. Se inicializa la posicion actual del
 * 				efecto y el contador de disparos e impactos. Se crea el tmr.
 * @param	*p_player	Asociado al objeto TipoPlayer
 */
void InicializaPlayer (TipoPlayer *p_player) {

    // Crear el timer
    p_player->tmr_duracion_nota = tmr_new(timer_player_duracion_nota_actual_isr);

    /* Crea los 3 efectos */
	InicializaEfecto(&(p_player->efecto_disparo), "efecto_disparo", frecuenciasDisparo, tiemposDisparo, 16);
	InicializaEfecto(&(p_player->efecto_impacto), "efecto_impacto", frecuenciasImpacto, tiemposImpacto, 32);
	InicializaEfecto(&(p_player->efecto_fondo), "efecto_fondo", frecuenciaStarwars, tiempoStarwars, 59);

	/* Crea un string que se pasará al LCD */
	strCancion = "StrWr ";

	/* Apunta inicialmente el efecto a efecto_fondo */
	p_player->p_efecto = &p_player->efecto_fondo;

	/* Inicia la posicion de la nota a 0, y se posiciona en la celda 0 de cada array */
	p_player->posicion_nota_actual = 0;
	p_player->duracion_nota_actual = p_player->p_efecto->duraciones[p_player->posicion_nota_actual];
	p_player->frecuencia_nota_actual = p_player->p_efecto->frecuencias[p_player->posicion_nota_actual];

	/* Guarda los valores para cuando se interrumpa el efecto fondo */
	p_player->posicion_nota_save = p_player->posicion_nota_actual;
	p_player->duracion_nota_save = p_player->duracion_nota_actual;
	p_player->frecuencia_nota_save = p_player->frecuencia_nota_actual;

	softToneCreate(PLAYER_PWM_PIN);

	/* Contador de impactos y disparos a 0 */
	impactos = 0;
	disparos = 0;
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

/* CompruebaStartDisparo : CompruebaStartImpacto : CompruebaNuevaNota :
 * CompruebaNotaTimeout : CompruebaFinalEfecto
 * @Brief		Comprueba si los flags correspondientes estan activos.
 * @Param	this	Asociado al objeto TipoPlayer
 * @Return		Devuelve 1 si esta activo, 0 si no. Excepto en
 * 				CompruebaNuevaNota, que devuelve lo contrario.
 */
int CompruebaStartDisparo (fsm_t* this) {
	int result = 0;
	piLock(PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_START_DISPARO);
	piUnlock(PLAYER_FLAGS_KEY);
	return result;
}

int CompruebaStartImpacto (fsm_t* this) {
	int result = 0;
	piLock(PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_START_IMPACTO);
	piUnlock(PLAYER_FLAGS_KEY);
	return result;
}

int CompruebaNuevaNota (fsm_t* this){
	int result = 0;
	// OJO AL !result; FLAG_PLAYER_END == 0
	piLock(PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_PLAYER_END);
	piUnlock(PLAYER_FLAGS_KEY);
	return !result;
}

int CompruebaNotaTimeout (fsm_t* this) {
	int result = 0;
	piLock(PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_NOTA_TIMEOUT);
	piUnlock(PLAYER_FLAGS_KEY);
	return result;
}

int CompruebaFinalEfecto (fsm_t* this) {
	int result = 0;
	// FLAG_PLAYER_END == 1
	piLock(PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_PLAYER_END);
	piUnlock(PLAYER_FLAGS_KEY);
	return result;
}

/* ------------------ Mejora musica de fondo ------------------- */

/* @Brief		Comprueba si el juego esta activo
 * @Param	this	Asociado al objeto TipoPlayer
 * @Return		Devuelve 1 si esta activo, 0 si no.
 */
int CompruebaJuegoON (fsm_t* this) {
	int result = 0;
	// FLAG_JUEGO_ON == 1
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_JUEGO_ON);
	piUnlock(SYSTEM_FLAGS_KEY);
	return result;
}

/* @Brief		Comprueba si el juego esta activo
 * @Param	this	Asociado al objeto TipoPlayer
 * @Return		Devuelve 1 si esta NO activo, 0 si lo esta.
 */
int CompruebaJuegoOFF (fsm_t* this) {
	int result = 0;
	// OJO AL !result; FLAG_JUEGO_ON == 0
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_JUEGO_ON);
	piUnlock(SYSTEM_FLAGS_KEY);
	return !result;
}

/* @Brief		Comprueba si se quiere cambiar de cancion de fondo
 * @Param	this	Asociado al objeto TipoPlayer
 * @Return		Devuelve 1 si se va a cambiar, 0 si no.
 */
int CompruebaCambioCancion (fsm_t* this) {
	int result = 0;
	piLock(PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_CAMBIO_C);
	piUnlock(PLAYER_FLAGS_KEY);
	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCI0N DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

/* @Brief		Inicia el efecto Disparo. Saca por pantalla del LCD
 * 				el numero de disparos. Se aumenta en 1 los disparos.
 * @Param	this	Asociado al objeto TipoPlayer
 */
void InicializaPlayDisparo (fsm_t* this) {

	TipoPlayer *p_player;
	p_player = (TipoPlayer*)(this->user_data);

	/* Indica que el efecto a reproducir no es la musica de fondo */
	fondo = 0;

	piLock (PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_START_DISPARO;
	flags_player &= ~FLAG_PLAYER_END;
	piUnlock (PLAYER_FLAGS_KEY);

	/* Guarda los valores ya que se ha interrumpido el efecto fondo */
	p_player->posicion_nota_save = p_player->posicion_nota_actual;
	p_player->duracion_nota_save = p_player->duracion_nota_actual;
	p_player->frecuencia_nota_save = p_player->frecuencia_nota_actual;

	/* Apunta el efecto a efecto_disparo */
	p_player->p_efecto = &(p_player->efecto_disparo);

	/* Inicia la posicion de la nota a 0, y se posiciona en la celda 0 de cada array */
	p_player->posicion_nota_actual = 0;
	p_player->duracion_nota_actual = p_player->p_efecto->duraciones[p_player->posicion_nota_actual];
	p_player->frecuencia_nota_actual = p_player->p_efecto->frecuencias[p_player->posicion_nota_actual];

	/* Empieza el temporizador de nota */
	tmr_startms(p_player->tmr_duracion_nota, p_player->duracion_nota_actual);

	piLock(STD_IO_BUFFER_KEY);
	printf("Reproduciendo la primera nota. Frecuencia: %d. Num notas = %d.\n", p_player->frecuencia_nota_actual, p_player->p_efecto->num_notas);
	piUnlock(STD_IO_BUFFER_KEY);

	/* Aumenta el contador de disparos en 1 */
	disparos++;

	/* Reproduce la nota inicial */
	softToneWrite(PLAYER_PWM_PIN,p_player->frecuencia_nota_actual);

	/* Saca por la pantalla del LCD el contador de disparos */
	lcdClear(lcdControl2);
	lcd(0,lcdControl2);
}

/* @Brief		Inicia el efecto Impacto. Saca por pantalla del LCD
 * 				el numero de impactos. Se aumenta en 1 los impactos.
 * @Param	this	Asociado al objeto TipoPlayer
 */
void InicializaPlayImpacto (fsm_t* this) {

	TipoPlayer *p_player;
	p_player = (TipoPlayer*)(this->user_data);

	/* Indica que el efecto a reproducir no es la musica de fondo */
	fondo = 0;

	piLock (PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_START_IMPACTO;
	flags_player &= ~FLAG_PLAYER_END;
	piUnlock (PLAYER_FLAGS_KEY);

	/* Apunta el efecto a efecto_impacto */
	p_player->p_efecto = &(p_player->efecto_impacto);

	/* Inicia la posicion de la nota a 0, y se posiciona en la celda 0 de cada array */
	p_player->posicion_nota_actual = 0;
	p_player->duracion_nota_actual = p_player->p_efecto->duraciones[p_player->posicion_nota_actual];
	p_player->frecuencia_nota_actual = p_player->p_efecto->frecuencias[p_player->posicion_nota_actual];

	/* Empieza el temporizador de nota */
	tmr_startms(p_player->tmr_duracion_nota, p_player->duracion_nota_actual);

	piLock(STD_IO_BUFFER_KEY);
	printf("¡Se ha detectado un impacto!\n");
	printf("Reproduciendo la primera nota. Frecuencia: %d. Num notas = %d.\n", p_player->frecuencia_nota_actual, p_player->p_efecto->num_notas);
	piUnlock(STD_IO_BUFFER_KEY);

	/* Reproduce la nota inicial */
	softToneWrite(PLAYER_PWM_PIN,p_player->frecuencia_nota_actual);

	/* Aumenta el contador de impactos en 1 */
	impactos++;

	/* Saca por la pantalla del LCD el contador de impactos */
	lcdClear(lcdControl2);
	lcd(0,lcdControl2);
}

/* @Brief		Inicia el efecto Fondo. Si se dispara, se guardara por
 * 				donde iba la cancion para recuperar la posicion de los arrays
 * 				al terminar otros efectos.
 * @Param	this	Asociado al objeto TipoPlayer
 */
void InicializaPlayFondo (fsm_t* this) {

	TipoPlayer *p_player;
	p_player = (TipoPlayer*)(this->user_data);

	/* Indica que el efecto a reproducir es la musica de fondo */
	fondo = 1;

	piLock (PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_PLAYER_END;
	piUnlock (PLAYER_FLAGS_KEY);

	/* Apunta el efecto a efecto_fondo */
	p_player->p_efecto = &(p_player->efecto_fondo);

	/* Inicia la posicion de la nota a 0, y se posiciona en la celda 0 de cada array si es
	 * la primera vez que se reproduce. Si no, recupera la posicion anterior a ser
	 * interrumpida por un disparo */
	p_player->posicion_nota_actual = p_player->posicion_nota_save;
	p_player->duracion_nota_actual = p_player->duracion_nota_save;
	p_player->frecuencia_nota_actual = p_player->frecuencia_nota_save;

	/* Empieza el temporizador de nota */
	tmr_startms(p_player->tmr_duracion_nota, p_player->duracion_nota_actual);

	/* piLock(STD_IO_BUFFER_KEY);
	printf("Reproduciendo la primera nota. Frecuencia: %d. Num notas = %d.\n", p_player->frecuencia_nota_actual, p_player->p_efecto->num_notas);
	piUnlock(STD_IO_BUFFER_KEY); */

	/* Reproduce la nota inicial */
	softToneWrite(PLAYER_PWM_PIN,p_player->frecuencia_nota_actual);

}

/* @Brief		Reproduce la siguiente nota del efecto actual
 * @Param	this	Asociado al objeto TipoPlayer
 */
void ComienzaNuevaNota (fsm_t* this) {

	TipoPlayer *p_player;
	p_player = (TipoPlayer*)(this->user_data);

	piLock(PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_PLAYER_END;
	piUnlock(PLAYER_FLAGS_KEY);

	/* Comprueba si el efecto no es fondo para mostrar por pantalla las notas
	 * reproducidas. Se hace para no saturar la pantalla. */
	if(!fondo) {
		piLock(STD_IO_BUFFER_KEY);
		printf("Reproduciendo la nota numero %d, de frecuencia %d. Posicion nota actual: %d. \n", p_player->posicion_nota_actual, p_player->frecuencia_nota_actual, p_player->posicion_nota_actual);
		piUnlock(STD_IO_BUFFER_KEY);
	}

	/* Empieza el temporizador de nota */
	tmr_startms(p_player->tmr_duracion_nota, p_player->duracion_nota_actual);

	/* Reproduce la nota actual */
	softToneWrite(PLAYER_PWM_PIN,p_player->frecuencia_nota_actual);

}

/* @Brief		Comprueba si quedan notas, si quedan apunta a la nota siguiente.
 * @Param	this	Asociado al objeto TipoPlayer
 */
void ActualizaPlayer (fsm_t* this) {

	TipoPlayer *p_player;
	p_player = (TipoPlayer*)(this->user_data);

	/* Desactiva el flag temporizador */
	piLock(PLAYER_FLAGS_KEY);
	flags_player &= (~FLAG_NOTA_TIMEOUT);
	piUnlock(PLAYER_FLAGS_KEY);

	/* Pasa a apuntar a la siguiente nota del efecto */
	p_player->posicion_nota_actual++;
	p_player->duracion_nota_actual = p_player->p_efecto->duraciones[p_player->posicion_nota_actual];
	p_player->frecuencia_nota_actual = p_player->p_efecto->frecuencias[p_player->posicion_nota_actual];

	/* Si la nota es la ultima nota del efecto, termina de reproducir */
	if (p_player->p_efecto->num_notas <= p_player->posicion_nota_actual ) {
		/* Activa el flag que indica que ha acabado de reproducirse el efecto */
		piLock(PLAYER_FLAGS_KEY);
		flags_player |= FLAG_PLAYER_END;
		piUnlock(PLAYER_FLAGS_KEY);
		/* Comprueba si el efecto no es fondo para mostrar por pantalla las notas
		 * reproducidas. Se hace para no saturar la pantalla. */
		if (!fondo) {
			piLock(STD_IO_BUFFER_KEY);
			printf("La última frecuencia reproducida es %d.\n", p_player->frecuencia_nota_actual);
			piUnlock(STD_IO_BUFFER_KEY);
		}
		return;
	}
}

/* @Brief		Reproduce la frecuencia 0, pone la nota actual a 0 y
 * reinicia valores de arrays. Comprueba limite de disparos e impactos.
 * @Param	this	Asociado al objeto TipoPlayer
 */
void FinalEfecto (fsm_t* this) {

	TipoPlayer *p_player;
    p_player = (TipoPlayer*)(this->user_data);

	piLock(PLAYER_FLAGS_KEY);
	flags_player &= (~FLAG_PLAYER_END);
	piUnlock(PLAYER_FLAGS_KEY);

	/* Reinicio de los valores de los efectos */
	p_player->posicion_nota_actual = 0;
	p_player->duracion_nota_actual = p_player->p_efecto->duraciones[p_player->posicion_nota_actual];
	p_player->frecuencia_nota_actual = p_player->p_efecto->frecuencias[p_player->posicion_nota_actual];

	/* Reinicio de los valores guardados para fondo */
	if (fondo) {
		p_player->posicion_nota_save = p_player->posicion_nota_actual;
		p_player->duracion_nota_save = p_player->duracion_nota_actual;
		p_player->frecuencia_nota_save = p_player->frecuencia_nota_actual;
	}

	piLock(STD_IO_BUFFER_KEY);
	if (!fondo) {
		printf("Evento acabado.\n\n");
	} else {
		printf("Cancion acabada.\n\n");
	}
	piUnlock(STD_IO_BUFFER_KEY);

	/* Se reproducira ahora el efecto de fondo, asi que activo la variable */
    fondo = 1;



	/* Reproduce la frecuencia 0 */
	softToneWrite(PLAYER_PWM_PIN,0);

	/* Comprueba limite de impactos, si se supera, acaba el juego */
	if (impactos >= METAIMPACTOS) {
		piLock (SYSTEM_FLAGS_KEY);
		flags_juego |= FLAG_SYSTEM_START_END;
		piUnlock (SYSTEM_FLAGS_KEY);
	}
	/* Comprueba limite de disparos, si se supera, acaba el juego */
	if (disparos >= MAXDISPAROS) {
		piLock (SYSTEM_FLAGS_KEY);
		flags_juego |= FLAG_SYSTEM_START_END;
		piUnlock (SYSTEM_FLAGS_KEY);
	}
}

/* @Brief		Cambia la cancion de fondo y muestra cual es por el LCD.
 * @Note		El efecto que apunta a efecto_fondo cambia sus variables
 * 				cada vez que se cambia de cancion.
 * @Param	this	Asociado al objeto TipoPlayer
 */
void CambioCancion (fsm_t* this) {

	TipoPlayer *p_player;
	p_player = (TipoPlayer*)(this->user_data);

	/* Aumento el contador de cambio de cancion */
	cambioC++;
	/* Reinicio el contador de cambio, ya que solo hay 4 canciones */
	if (cambioC >= 4)
		cambioC = 0;

	switch(cambioC){
	case 0:
		InicializaEfecto(&(p_player->efecto_fondo), "efecto_fondo", frecuenciaStarwars, tiempoStarwars, 59);
		//printf("\nSonando: Star Wars\n");
		strCancion = "StrWr ";
		lcdClear(lcdControl2);
		lcd(0,lcdControl2);
		break;
	case 1:
		InicializaEfecto(&(p_player->efecto_fondo), "efecto_fondo", frecuenciaDespacito, tiempoDespacito, 160);
		//printf("\nSonando: Despacito\n");
		strCancion = "Despa ";
		lcdClear(lcdControl2);
		lcd(0,lcdControl2);
		break;
	case 2:
		InicializaEfecto(&(p_player->efecto_fondo), "efecto_fondo", frecuenciaTetris, tiempoTetris, 55);
		//printf("\nSonando: Tetris\n");
		strCancion = "Tetri ";
		lcdClear(lcdControl2);
		lcd(0,lcdControl2);
		break;
	case 3:
		InicializaEfecto(&(p_player->efecto_fondo), "efecto_fondo", frecuenciaGOT, tiempoGOT, 518);
		//printf("\nSonando: GOT\n");
		strCancion = "GOT   ";
		lcdClear(lcdControl2);
		lcd(0,lcdControl2);
		break;
	default:
		break;
	}

    piLock(PLAYER_FLAGS_KEY);
	flags_player &= (~FLAG_CAMBIO_C);
	piUnlock(PLAYER_FLAGS_KEY);
}

//------------------------------------------------------
// PROCEDIMIENTOS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

/* @Brief		Desactiva el flag asociado al temporizador
 * 				encargado de cambiar de nota.
 */
void timer_player_duracion_nota_actual_isr (union sigval value) {

	piLock(PLAYER_FLAGS_KEY);
	flags_player |= FLAG_NOTA_TIMEOUT;
	piUnlock(PLAYER_FLAGS_KEY);

}


/**************************************************************************/
