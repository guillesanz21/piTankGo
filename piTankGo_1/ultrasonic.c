/** File Name		: ultrasonic.c
 *  Description		: Implementación del módulo de ultrasonidos HC-SR04 para
 *  medir la distancia a la que se encuentra la torreta del objeto que tiene
 *  en la direccion a la que apunta la torreta. Cuando se encuentre un objeto
 *  a menor distancia de la establecida, activará el flag de disparo.
 */

/* Includes -----------------------------------------------------------------*/
#include "ultrasonic.h"

/* @Brief		Método encargado configurar e inicializar los pines del US, ademas
 * 				de crear la hebra encargada de realizar la medicion de la distancia.
 * @Return		Devuelve -1 si la hebra no se ha iniciado correctamente
 */
int ConfiguraUS() {

	int result = 0;
	pinMode(US_TRIG, OUTPUT);
	pinMode(US_ECHO, INPUT);
	digitalWrite(US_TRIG, LOW);
	result = piThreadCreate (thread_ultrasonidos);
	if (result != 0) {
		printf ("Thread didn't start!!!\n");
		return -1;
  }
  return result;
}

/* @Brief		Hebra encargada de realizar la medicion de distancia y comprobar si
 * esa distancia es menor a la establecida, en ese caso, activara el flag de disparo.
 * @Param thread_ultrasonidos		nombre de la hebra.
 */
PI_THREAD (thread_ultrasonidos) {

	/* Bucle infinito */
	while (1) {

		/* Realizamos la medicion cada CICLO_US segundos (para no saturar el sistema) */
		delay(CICLO_US);

		piLock (STD_IO_BUFFER_KEY);

		/* Ponemos a 1 el pin encargado de emitir los ultrasonidos */
		digitalWrite(US_TRIG, HIGH);
		/* Le damos 20 ms para asegurar un 1 a la salida del pin del trigger */
		delayMicroseconds(20);
		digitalWrite(US_TRIG, LOW); 		// Ponemos a 0 el pin

		/* Esperamos hasta que empiece la medición */
		while(digitalRead(US_ECHO) == LOW){	}
		/* Guarda el instante en el que comienza la medición */
		ultrasonido.t_inicio = micros();
		/* Esperamos mientras realiza la medición */
		while(digitalRead(US_ECHO) == HIGH) { }
		/* Guarda el instante en el que termina la medicion */
		ultrasonido.t_final = micros();

		/* Velocidad sonido = 340.29 m/s = 0.0343 cm/us => La distancia recorrida
		 * será de ida y vuelta, pero solo nos interesa medir la distancia de ida,
		 * como son similares => 34.029/2
		 */
		ultrasonido.distance = (ultrasonido.t_final - ultrasonido.t_inicio) * 0.0343/2;

		if(juego_ON == 1){
			USdistance = ultrasonido.distance;
			/* Saca por pantalla del LCD la distancia medida */
			lcd(0,lcdControl2);
		}

		/* Si detecta un objeto a menos de 7 centimetros, disparará. Debe cumplir
		 * que el juego este activo */
		if (ultrasonido.distance < DISTANCE_SHOOT && juego_ON == 1) {
			/*piLock(STD_IO_BUFFER_KEY);
			printf("\nDistance: %fcm\n", ultrasonido.distance);
			piUnlock(STD_IO_BUFFER_KEY);*/
			piLock (SYSTEM_FLAGS_KEY);
		    flags_juego |= FLAG_TRIGGER_BUTTON;
		    piUnlock (SYSTEM_FLAGS_KEY);
		}
		piUnlock (STD_IO_BUFFER_KEY);
	}
}

/**************************************************************************/
