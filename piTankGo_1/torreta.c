/** File Name		: torreta.c
 *  Description		: Movimiento y disparo de la torreta.
 *  Estos movimientos se hacen a través de servos, que son controlados
 *  por un joystick.
 */

#include "torreta.h"


//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

/*
 * @Brief		Configura los pines utilizados en la torreta
 * @Param	this	Asociado al objeto TipoTorreta
 */
void ConfiguraTorreta (TipoTorreta *p_torreta) {
	pinMode(SERVO_VERTICAL_PIN, OUTPUT);
	pinMode(SERVO_HORIZONTAL_PIN, OUTPUT);
}

/*
 * @Brief		Crea e inicializa las señales PWM. Crea el temporizador
 * @Note		La torreta usa servos controlados por señales PWM
 * @Param	this	Asociado al objeto TipoTorreta
 */
void InicializaTorreta (TipoTorreta *p_torreta) {

	// Internamente ya hace: piHiPri (90) ;
	softPwmCreate (SERVO_VERTICAL_PIN, p_torreta->servo.inicio, SERVO_PWM_RANGE);
	softPwmWrite(SERVO_VERTICAL_PIN, p_torreta->posicion.x);

	// Internamente ya hace: piHiPri (90) ;
	softPwmCreate (SERVO_HORIZONTAL_PIN, p_torreta->servo.inicio, SERVO_PWM_RANGE);
	softPwmWrite(SERVO_HORIZONTAL_PIN, p_torreta->posicion.y);

	p_torreta->tmr_torreta = tmr_new(timer_duracion_disparo_isr);

}

//---------------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//---------------------------------------------------------------

/* CompruebaComienzo : CompruebaJoystickUP : CompruebaJoystickDOWN :
 * CompruebaJoystickRIGHT : CompruebaJoystickLEFT : CompruebaTimeoutDisparo :
 * CompruebaImpacto : CompruebaTriggerButton : CompruebaFinalJuego
 * @Brief		Comprueba si los flags correspondientes estan activos.
 * @Param	this	Asociado al objeto TipoTorreta
 * @Return		Devuelve 1 si esta activo, 0 si no.
 */
int CompruebaComienzo (fsm_t* this) {
	int result = 0;
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_SYSTEM_START_END);
	piUnlock(SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaJoystickUp (fsm_t* this) {
	int result = 0;
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_JOYSTICK_UP);
	piUnlock(SYSTEM_FLAGS_KEY);
	return result;
}

int CompruebaJoystickDown (fsm_t* fsm_player){
	int result = 0;
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_JOYSTICK_DOWN);
	piUnlock(SYSTEM_FLAGS_KEY);
	return result;
}

int CompruebaJoystickLeft (fsm_t* this) {
	int result = 0;
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_JOYSTICK_LEFT);
	piUnlock(SYSTEM_FLAGS_KEY);
	return result;
}

int CompruebaJoystickRight (fsm_t* this) {
	int result = 0;
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_JOYSTICK_RIGHT);
	piUnlock(SYSTEM_FLAGS_KEY);
	return result;
}

int CompruebaTimeoutDisparo (fsm_t* this) {
	int result = 0;
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_SHOOT_TIMEOUT);
	piUnlock(SYSTEM_FLAGS_KEY);
	return result;
}

int CompruebaImpacto (fsm_t* this) {
	int result = 0;
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_TARGET_DONE);
	piUnlock(SYSTEM_FLAGS_KEY);
	return result;
}

int CompruebaTriggerButton (fsm_t* this) {
	int result = 0;
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_TRIGGER_BUTTON);
	piUnlock(SYSTEM_FLAGS_KEY);
	return result;
}

/* Un solo Flag es suficiente para tratar el start y el stop del juego */
int CompruebaFinalJuego (fsm_t* this) {
	int result = 0;
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_SYSTEM_START_END);
	piUnlock(SYSTEM_FLAGS_KEY);
	return result;
}

//----------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//----------------------------------------------------------

/* @Brief		Establece los valores de inicio: Inicializa variables
 * 				de posicion y mueve la torreta a la posicion inicial.
 * @Note		Activa la variable y el flag de juego ON
 * @Param	this	Asociado al objeto TipoTorreta
 */
void ComienzaSistema (fsm_t* this) {

	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	/* Incremento maximo del movimiento de cada servo */
	p_torreta->servo.incremento = SERVO_INCREMENTO;

	/* Rango de movimiento de cada servo */
	p_torreta->servo.minimo = SERVO_MINIMO;
	p_torreta->servo.maximo = SERVO_MAXIMO;

	/* Posicion de inicio de cada servo */
	p_torreta->servo.inicio = SERVO_MINIMO + (SERVO_MAXIMO - SERVO_MINIMO)/2;
	p_torreta->posicion.x= p_torreta->servo.inicio;
	p_torreta->posicion.y= p_torreta->servo.inicio;

	/* Limites en el rango de movimiento */
	if(p_torreta->posicion.x > p_torreta->servo.maximo)
		p_torreta->posicion.x = p_torreta->servo.maximo;

	if(p_torreta->posicion.x < p_torreta->servo.minimo)
		p_torreta->posicion.x = p_torreta->servo.minimo;

	if(p_torreta->posicion.y > p_torreta->servo.maximo)
		p_torreta->posicion.y = p_torreta->servo.maximo;

	if(p_torreta->posicion.y < p_torreta->servo.minimo)
		p_torreta->posicion.y = p_torreta->servo.minimo;


	/* Mueve la torreta a la posicion inicial */
	softPwmWrite(SERVO_HORIZONTAL_PIN, p_torreta->posicion.x);
	softPwmWrite(SERVO_VERTICAL_PIN, p_torreta->posicion.y);

	piLock (STD_IO_BUFFER_KEY);
	printf("\nSe ha inicializado la torreta\n");
	//printf("\nSonando: Star Wars\n");
	printf("[SERVO][POSICION]=[%d][%d]\n", p_torreta->posicion.x, p_torreta->posicion.y);
	fflush(stdout);
	piUnlock (STD_IO_BUFFER_KEY);

	piLock(SYSTEM_FLAGS_KEY);
	flags_juego &= (~FLAG_SYSTEM_START_END);
	piUnlock(SYSTEM_FLAGS_KEY);

	juego_ON = 1;

	piLock(SYSTEM_FLAGS_KEY);
	flags_juego |= FLAG_JUEGO_ON;
	piUnlock(SYSTEM_FLAGS_KEY);

}

/* MueveTorretaArriba : MueveTorretaAbajo : MueveTorretaIzquierda : MueveTorretaDerecha
 * @Brief		Mueve la torreta a la posicion correspondiente. Esto se consigue sumando
 * 				el incremento a la posicion anterior.
 * @Param	this	Asociado al objeto TipoTeclado
 */
void MueveTorretaArriba (fsm_t* this) {

	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	piLock(SYSTEM_FLAGS_KEY);
	flags_juego &= (~FLAG_JOYSTICK_UP);
	piUnlock(SYSTEM_FLAGS_KEY);

	/* Mueve el servo */
	if(p_torreta->posicion.y + p_torreta->servo.incremento <= p_torreta->servo.maximo) {
		p_torreta->posicion.y =  p_torreta->posicion.y + p_torreta->servo.incremento;

		softPwmWrite(SERVO_VERTICAL_PIN, p_torreta->posicion.y);

		piLock (STD_IO_BUFFER_KEY);
		printf("[SERVO][POSICION]=[%d][%d]\n", p_torreta->posicion.x, p_torreta->posicion.y);
		fflush(stdout);
		piUnlock (STD_IO_BUFFER_KEY);
	}
}

void MueveTorretaAbajo (fsm_t* this) {

	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	piLock(SYSTEM_FLAGS_KEY);
	flags_juego &= (~FLAG_JOYSTICK_DOWN);
	piUnlock(SYSTEM_FLAGS_KEY);

	/* Mueve el servo */
	if(p_torreta->posicion.y - p_torreta->servo.incremento >= p_torreta->servo.minimo) {
		p_torreta->posicion.y =  p_torreta->posicion.y - p_torreta->servo.incremento;

		softPwmWrite(SERVO_VERTICAL_PIN, p_torreta->posicion.y);

		piLock (STD_IO_BUFFER_KEY);
		printf("[SERVO][POSICION]=[%d][%d]\n", p_torreta->posicion.x, p_torreta->posicion.y);
		piUnlock (STD_IO_BUFFER_KEY);
		fflush(stdout);
	}
}

void MueveTorretaIzquierda (fsm_t* this) {

	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	piLock(SYSTEM_FLAGS_KEY);
	flags_juego &= (~FLAG_JOYSTICK_LEFT);
	piUnlock(SYSTEM_FLAGS_KEY);

	/* Mueve el servo */
	if(p_torreta->posicion.x - p_torreta->servo.incremento >= p_torreta->servo.minimo) {
		p_torreta->posicion.x =  p_torreta->posicion.x - p_torreta->servo.incremento;

		softPwmWrite(SERVO_HORIZONTAL_PIN, p_torreta->posicion.x);

		piLock (STD_IO_BUFFER_KEY);
		printf("[SERVO][POSICION]=[%d][%d]\n", p_torreta->posicion.x, p_torreta->posicion.y);
		piUnlock (STD_IO_BUFFER_KEY);
		fflush(stdout);
	}
}

void MueveTorretaDerecha (fsm_t* this) {

	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	piLock(SYSTEM_FLAGS_KEY);
	flags_juego &= (~FLAG_JOYSTICK_RIGHT);
	piUnlock(SYSTEM_FLAGS_KEY);

	/* Mueve el servo */
	if(p_torreta->posicion.x + p_torreta->servo.incremento <= p_torreta->servo.maximo) {
		p_torreta->posicion.x =  p_torreta->posicion.x + p_torreta->servo.incremento;

		softPwmWrite(SERVO_HORIZONTAL_PIN, p_torreta->posicion.x);

		piLock (STD_IO_BUFFER_KEY);
		printf("[SERVO][POSICION]=[%d][%d]\n", p_torreta->posicion.x, p_torreta->posicion.y);
		piUnlock (STD_IO_BUFFER_KEY);
		fflush(stdout);
	}
}

/* @Brief		Dispara el infrarrojo y activa el disparo (Player)
 * @Note		Inicia el temporizador.
 * @Param	this	Asociado al objeto TipoTeclado
 */
void DisparoIR (fsm_t* this) {

	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	piLock(SYSTEM_FLAGS_KEY);
	flags_juego &= (~FLAG_TRIGGER_BUTTON);
	piUnlock(SYSTEM_FLAGS_KEY);

	piLock(PLAYER_FLAGS_KEY);
	flags_player |= FLAG_START_DISPARO;
	piUnlock(PLAYER_FLAGS_KEY);

	/* Sentencia correspondiente a la mejora del IR: Activa el LED emisor IR. */
    digitalWrite (IR_TX_PIN, HIGH);

    /* Disparo disponible cada SHOOTING_PERIOD segundo */
	tmr_startms(p_torreta->tmr_torreta,SHOOTING_PERIOD);

}

/* @Brief		Temporizador acabado.
 * @Note		Desactiva el flag de disparo (Player). Desactiva el LED IR Tx.
 * @Param	this	Asociado al objeto TipoTeclado
 */
void FinalDisparoIR (fsm_t* this) {

	piLock(SYSTEM_FLAGS_KEY);
	flags_juego &= (~FLAG_SHOOT_TIMEOUT);
	piUnlock(SYSTEM_FLAGS_KEY);
	/* Sentencia correspondiente a la mejora del IR: Desactiva el LED emisor IR */
    digitalWrite (IR_TX_PIN, LOW);
}

/* @Brief		Impacto realizado.
 * @Note		Activa el flag de impacto (Player). Desactiva el temporizador.
 * 				Desactiva el LED IR Tx.
 * @Param	this	Asociado al objeto TipoTeclado
 */
void ImpactoDetectado (fsm_t* this) {

	piLock(SYSTEM_FLAGS_KEY);
	flags_juego &= (~FLAG_TARGET_DONE);
	piUnlock(SYSTEM_FLAGS_KEY);

	piLock(SYSTEM_FLAGS_KEY);
	flags_juego &= (~FLAG_SHOOT_TIMEOUT);
	piUnlock(SYSTEM_FLAGS_KEY);

	piLock(PLAYER_FLAGS_KEY);
	flags_player |= FLAG_START_IMPACTO;
	piUnlock(PLAYER_FLAGS_KEY);

	/* Sentencia correspondiente a la mejora del IR: Desactiva el LED emisor IR */
    digitalWrite (IR_TX_PIN, LOW);
}

/* @Brief		El juego finaliza.
 * @Param	this	Asociado al objeto TipoTeclado
 */
void FinalizaJuego (fsm_t* this) {

	piLock(SYSTEM_FLAGS_KEY);
	flags_juego &= (~FLAG_SYSTEM_START_END);
	printf("\nSe ha acabado el juego\n");
	piUnlock(SYSTEM_FLAGS_KEY);

	piLock(STD_IO_BUFFER_KEY);
	if (impactos == 1)
		printf("Se han realizado %d disparos, ha impactado %d. \n", disparos, impactos);
	else
		printf("Se han realizado %d disparos, han impactado %d. \n", disparos, impactos);
	piUnlock(STD_IO_BUFFER_KEY);

	juego_ON = 0;
	piLock(SYSTEM_FLAGS_KEY);
	flags_juego &= (~FLAG_JUEGO_ON);
	piUnlock(SYSTEM_FLAGS_KEY);

	/* Muestra por el LCD que el juego ha terminado */
    lcdClear(lcdControl2);
	lcd(3,lcdControl2);
}

/* @Brief		Se reinicia el juego: reinicio de las variables de control del juego.
 * @Param	this	Asociado al objeto TipoTorreta
 */
void ReiniciaJuego (fsm_t* this) {
	impactos = 0;
	disparos = 0;
	juego_ON = 1;
}

/* @Brief		Desactiva el flag asociado al temporizador
 * 				encargado de apagar el disparo.
 */
void timer_duracion_disparo_isr (union sigval value){
	piLock(SYSTEM_FLAGS_KEY);
	flags_juego |= FLAG_SHOOT_TIMEOUT;
	piUnlock(SYSTEM_FLAGS_KEY);
}


/**************************************************************************/

