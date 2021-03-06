/** File Name		: piTankGo_1.c
 *  Description		: Fichero principal. Aqui se inicializan todos los parametros y
 *  llama a los m�todos de inicializaci�n de otros ficheros. Aqu� se crean las
 *  m�quinas de estados. Tambi�n se crea la hebra del teclado del PC.
 */

/* Includes -----------------------------------------------------------------*/
#include "piTankGo_1.h"

/* Variables --------------------------------------------------------*/
int frecuenciaDespacito[160] = {0,1175,1109,988,740,740,740,740,740,740,988,988,988,988,880,988,784,0,784,784,784,784,784,988,988,988,988,1109,1175,880,0,880,880,880,880,880,1175,1175,1175,1175,1318,1318,1109,0,1175,1109,988,740,740,740,740,740,740,988,988,988,988,880,988,784,0,784,784,784,784,784,988,988,988,988,1109,1175,880,0,880,880,880,880,880,1175,1175,1175,1175,1318,1318,1109,0,1480,1318,1480,1318,1480,1318,1480,1318,1480,1318,1480,1568,1568,1175,0,1175,1568,1568,1568,0,1568,1760,1568,1480,0,1480,1480,1480,1760,1568,1480,1318,659,659,659,659,659,659,659,659,554,587,1480,1318,1480,1318,1480,1318,1480,1318,1480,1318,1480,1568,1568,1175,0,1175,1568,1568,1568,1568,1760,1568,1480,0,1480,1480,1480,1760,1568,1480,1318};
int tiempoDespacito[160] = {1200,600,600,300,300,150,150,150,150,150,150,150,150,300,150,300,343,112,150,150,150,150,150,150,150,150,300,150,300,300,150,150,150,150,150,150,150,150,150,300,150,300,800,300,600,600,300,300,150,150,150,150,150,150,150,150,300,150,300,343,112,150,150,150,150,150,150,150,150,300,150,300,300,150,150,150,150,150,150,150,150,150,300,150,300,450,1800,150,150,150,150,300,150,300,150,150,150,300,150,300,450,450,300,150,150,225,75,150,150,300,450,800,150,150,300,150,150,300,450,150,150,150,150,150,150,150,150,300,300,150,150,150,150,150,150,450,150,150,150,300,150,300,450,450,300,150,150,150,300,150,300,450,800,150,150,300,150,150,300,450};
int frecuenciaGOT[518] = {1568,0,1046,0,1244,0,1397,0,1568,0,1046,0,1244,0,1397,0,1175,0,1397,0,932,0,1244,0,1175,0,1397,0,932,0,1244,0,1175,0,1046,0,831,0,698,0,523,0,349,0,784,0,523,0,523,0,587,0,622,0,698,0,784,0,523,0,622,0,698,0,784,0,523,0,622,0,698,0,587,0,698,0,466,0,622,0,587,0,698,0,466,0,622,0,587,0,523,0,523,0,587,0,622,0,698,0,784,0,523,0,622,0,698,0,784,0,523,0,622,0,698,0,587,0,698,0,466,0,622,0,587,0,698,0,466,0,622,0,587,0,523,0,0,1568,0,0,1046,0,0,1244,0,0,1397,0,0,1568,0,0,1046,0,0,1244,0,0,1397,0,0,1175,0,587,0,622,0,587,0,523,0,587,0,784,0,880,0,932,0,1046,0,1175,0,0,1397,0,0,932,0,0,1244,0,0,1175,0,0,1397,0,0,932,0,0,1244,0,0,1175,0,0,1046,0,0,1568,0,0,1046,0,0,1244,0,0,1397,0,0,1568,0,0,1046,0,0,1244,0,0,1397,0,0,1175,0,880,0,784,0,932,0,1244,0,0,1397,0,0,932,0,0,1175,0,0,1244,0,0,1175,0,0,932,0,0,1046,0,0,2093,0,622,0,831,0,932,0,1046,0,622,0,831,0,1046,0,0,1865,0,622,0,784,0,831,0,932,0,622,0,784,0,932,0,0,1661,0,523,0,698,0,784,0,831,0,523,0,698,0,831,0,0,1568,0,1046,0,1244,0,1397,0,1568,0,1046,0,1244,0,1397,0,0,0,1661,0,1046,0,1175,0,1244,0,831,0,1175,0,1244,0,0,0,0,2489,0,0,0,0,2794,0,0,0,0,3136,0,0,2093,0,622,0,831,0,932,0,1046,0,622,0,831,0,1046,0,0,1865,0,622,0,784,0,831,0,932,0,622,0,784,0,932,0,0,1661,0,523,0,698,0,784,0,831,0,523,0,698,0,831,0,0,1568,0,1046,0,1244,0,1397,0,1568,0,1046,0,1244,0,1397,0,0,0,1661,0,1046,0,1175,0,1244,0,831,0,1175,0,1244,0,0,0,0,2489,0,1397,0,0,0,2350,0,0,0,2489,0,0,0,2350,0,0,0,0,2093,0,392,0,415,0,466,0,523,0,392,0,415,0,466,0,523,0,392,0,415,0,466,0,2093,0,1568,0,1661,0,1865,0,2093,0,1568,0,1661,0,1865,0,2093,0,1568,0,1661,0,1865};
int tiempoGOT[518] = {900,89,900,89,133,13,133,13,600,59,600,59,133,13,133,13,1400,1400,900,89,900,89,133,13,133,13,600,59,900,89,133,13,133,13,1200,116,267,28,267,28,267,28,900,89,900,89,1400,89,69,7,69,7,69,7,69,7,900,89,900,89,133,13,133,13,600,59,600,59,133,13,133,13,1800,1800,900,89,900,89,133,13,133,13,600,59,900,89,133,13,133,13,1200,2400,69,7,69,7,69,7,69,7,900,89,900,89,133,13,133,13,600,59,600,59,133,13,133,13,1800,1800,900,89,900,89,133,13,133,13,600,59,900,89,133,13,133,13,1200,2400,3600,900,89,900,900,89,900,133,13,150,133,13,150,600,59,600,600,59,600,133,13,150,133,13,150,1200,400,69,7,69,7,69,7,69,7,267,28,400,45,133,13,267,28,267,28,267,28,300,900,89,900,900,89,900,133,13,150,133,13,150,600,59,600,900,89,900,133,13,150,133,13,150,1200,1800,3600,900,89,900,900,89,900,133,13,150,133,13,150,600,59,600,600,59,600,133,13,150,133,13,150,1200,400,267,28,1200,400,133,13,133,13,150,900,89,900,900,89,900,600,59,600,267,28,300,600,59,600,267,28,300,1200,2400,3600,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,133,13,267,28,267,28,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,150,600,59,133,13,133,13,267,28,267,28,133,13,133,13,150,150,150,900,89,900,900,900,900,89,900,900,900,1200,2400,3600,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,150,150,600,59,133,13,133,13,267,28,267,28,133,13,133,13,150,150,150,600,212,133,13,150,150,267,28,300,300,400,45,450,450,133,13,150,150,150,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,267,28,267,28,133,13,2400,116,267,28,267,28,133,13,133,13,267,28,267,28,133,13,133,13,267,28,267,28,133,13,2400};
int frecuenciaTetris[55] = {1319,988,1047,1175,1047,988,880,880,1047,1319,1175,1047,988,988,1047,1175,1319,1047,880,880,0,1175,1397,1760,1568,1397,1319,1047,1319,1175,1047,988,988,1047,1175,1319,1047,880,880,0,659,523,587,494,523,440,415,659,523,587,494,523,659,880,831};
int tiempoTetris[55] = {450,225,225,450,225,225,450,225,225,450,225,225,450,225,225,450,450,450,450,450,675,450,225,450,225,225,675,225,450,225,225,450,225,225,450,450,450,450,450,450,900,900,900,900,900,900,1800,900,900,900,900,450,450,900,1800};
int frecuenciaStarwars[59] = {523,0,523,0,523,0,698,0,1046,0,0,880,0,784,0,1397,0,523,0,1760,0,0,880,0,784,0,1397,0,523,0,1760,0,0,880,0,784,0,1397,0,523,0,1760,0,0,880,0,1760,0,0,784,0,523,0,0,523,0,0,523,0};
int tiempoStarwars[59] = {134,134,134,134,134,134,536,134,536,134,134,134,134,134,134,536,134,402,134,134,429,357,134,134,134,134,536,134,402,134,134,429,357,134,134,134,134,536,134,402,134,134,429,357,134,134,134,429,357,1071,268,67,67,268,67,67,67,67,67};

int frecuenciasDisparo[16] = {2500,2400,2300,2200,2100,2000,1900,1800,1700,1600,1500,1400,1300,1200,1100,1000};
int tiemposDisparo[16] = {75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75};
int frecuenciasImpacto[32] = {97,109,79,121,80,127,123,75,119,96,71,101,98,113,92,70,114,75,86,103,126,118,128,77,114,119,72};
int tiemposImpacto[32] = {10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10};

/* Inicializacion de todas las variables externas creadas en piTankLib.h */
int flags_juego = 0;
int flags_player = 0;
int flags = 0;
int btn_flag = 0;
int juego_ON = 0;
int cambioC = 0;
int lcdControl2 = 0;
double USdistance = 0.0;
char *strCancion = "";


//------------------------------------------------------
// FUNCIONES DE CONFIGURACION/INICIALIZACION
//------------------------------------------------------

/* @Brief		Inicializa WiringPi, llama a todos los metodos de
 * configuracion de todos los modulos. Inicializa el LCD y el pin
 * de audio.
 * @Param	p_sistema		Asociado al objeto TipoSistema
 */
int ConfiguraSistema (TipoSistema *p_sistema) {
    int result = 0;

    /* Inicializa Wiring Pi en modo BCM */
    if (wiringPiSetupGpio() < 0)
        printf("wiringPiSetupGpio didn't start.");

    /* Llamada a todos los metodos de configuracion */
    // ConfiguraTeclado(&teclado);
    ConfiguraJoystick();
    ConfiguraTorreta(&(p_sistema->torreta));
    ConfiguraBTN();
    ConfiguraIR();
    ConfiguraUS();

    /* Configuracion del LCD */
    lcdControl2 = lcdInit (2, 16, 4,  RS,E , D4,D5,D6,D7,0,0,0,0) ;
    lcdClear(lcdControl2);
	lcd(1,lcdControl2);

	/* Configuracion del pin de audio de Player */
    pinMode(PLAYER_PWM_PIN, OUTPUT);

    return result;
}

/* @Brief		Inicializa el sistema (tanto la torreta como Player)
 * Ademas, crea la hebra del teclado del PC
 * @Param	p_sistema		Asociado al objeto TipoSistema
 */
int InicializaSistema (TipoSistema *p_sistema) {
    int result = 0;

    /* Llamada a los metodos de inicializacion de torreta y Player */
    InicializaPlayer(&(p_sistema->player));
    InicializaTorreta(&(p_sistema->torreta));

    /* Lanzamos thread para exploracion del teclado convencional del PC */
    /* result = piThreadCreate (thread_explora_teclado_PC);
    if (result != 0) {
        printf ("Thread didn't start!!!\n");
        return -1;
    } */

    return result;
}

//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

/* @Brief		Hebra encargada de realizar la lectura del teclado del PC y
 * asignar una funcion a cada tecla programada.
 * @Param thread_explora_teclado_PC		nombre de la hebra.
 */
PI_THREAD (thread_explora_teclado_PC) {
        int teclaPulsada;

        /* Bucle infinito */
        while(1) {
        	// Wiring Pi function: pauses program execution for at least 10 ms
            delay(10);
            piLock (STD_IO_BUFFER_KEY);
            if(kbhit()) {
            	/* Lee la tecla pulsada */
                teclaPulsada = kbread();
                switch(teclaPulsada) {
                    case 's':
                        piLock(SYSTEM_FLAGS_KEY);
                        flags_juego |= FLAG_SYSTEM_START_END;
                        piUnlock(SYSTEM_FLAGS_KEY);
                        //printf("Tecla S pulsada!\n");
                        lcdClear(lcdControl2);
                        fflush(stdout);
                        break;
                    case 'c':
                        piLock(PLAYER_FLAGS_KEY);
                    	flags_player |= FLAG_CAMBIO_C;
                    	piUnlock(PLAYER_FLAGS_KEY);
                    	//printf("Cambio de cancion\n");
                    	break;
                    case 'q':
                    	exit(0);
                        break;
                    default:
                        printf("INVALID KEY!!!\n");
                        break;
                }
            }
            piUnlock (STD_IO_BUFFER_KEY);
        }
}

/* @Brief		// wait until next_activation (absolute time)
 * @Param thread_explora_teclado_PC		Hasta cuando tiene que esperar.
 */
void delay_until (unsigned int next) {
    unsigned int now = millis();
    if (next > now) {
        delay (next - now);
    }
}

/* @Brief		Programa principal. Aqu� se crean e inicializan las
 * maquinas de estados.
 */
int main () {

    TipoSistema sistema;
    unsigned int next;

    /* Configuracion e inicializacion del sistema */
    ConfiguraSistema (&sistema);
    InicializaSistema (&sistema);

    /*--------------------- Maquinas de estados --------------------- */

    /* Maquinas de estados para el teclado matricial */
    /* fsm_trans_t columns[] = {
            { KEY_COL_1, CompruebaColumnTimeout, KEY_COL_2, col_2 },
            { KEY_COL_2, CompruebaColumnTimeout, KEY_COL_3, col_3 },
            { KEY_COL_3, CompruebaColumnTimeout, KEY_COL_4, col_4 },
            { KEY_COL_4, CompruebaColumnTimeout, KEY_COL_1, col_1 },
            {-1, NULL, -1, NULL },
    };

    fsm_trans_t keypad[] = {
            { KEY_WAITING, key_pressed, KEY_WAITING, process_key},
            {-1, NULL, -1, NULL },
    };

    fsm_t* columns_fsm = fsm_new (KEY_COL_1, columns, &teclado);
    fsm_t* keypad_fsm = fsm_new (KEY_WAITING, keypad, &teclado);
    */

    /* Maquina de estado para el Player */
    /* Se ha modificado respecto al enunciado para poder asi implementar
     * las mejoras de efecto de fondo y cambio de cancion */
    fsm_trans_t reproductor[] = {
    		{ WAIT_START, CompruebaJuegoON, WAIT_NEXT, InicializaPlayFondo },
			{ WAIT_NEXT, CompruebaStartDisparo, WAIT_NEXT, InicializaPlayDisparo },
            { WAIT_NEXT, CompruebaStartImpacto, WAIT_NEXT, InicializaPlayImpacto },
            { WAIT_NEXT, CompruebaNotaTimeout, WAIT_END, ActualizaPlayer },
			{ WAIT_END, CompruebaCambioCancion, WAIT_START, CambioCancion },
			{ WAIT_END, CompruebaJuegoOFF, WAIT_START, FinalEfecto },
			{ WAIT_END, CompruebaFinalEfecto, WAIT_START, FinalEfecto },
			{ WAIT_END, CompruebaNuevaNota, WAIT_NEXT, ComienzaNuevaNota},
            {-1, NULL, -1, NULL },
    };

    fsm_t* player_fsm = fsm_new (WAIT_START, reproductor, &(sistema.player));

    /* Maquina de estado para la Torreta */
    fsm_trans_t controlTorreta[] = {
    		{ WAIT_START, CompruebaComienzo, WAIT_MOVE, ComienzaSistema},
       		{ WAIT_MOVE, CompruebaJoystickUp, WAIT_MOVE, MueveTorretaArriba},
			{ WAIT_MOVE, CompruebaJoystickRight, WAIT_MOVE, MueveTorretaDerecha},
			{ WAIT_MOVE, CompruebaJoystickLeft, WAIT_MOVE, MueveTorretaIzquierda},
			{ WAIT_MOVE, CompruebaJoystickDown, WAIT_MOVE, MueveTorretaAbajo},
			{ WAIT_MOVE, CompruebaTriggerButton, TRIGGER_BUTTON, DisparoIR},
			{ TRIGGER_BUTTON, CompruebaImpacto, WAIT_MOVE, ImpactoDetectado},
			{ TRIGGER_BUTTON, CompruebaTimeoutDisparo, WAIT_MOVE, FinalDisparoIR},
       		{ WAIT_MOVE, CompruebaFinalJuego, WAIT_END, FinalizaJuego},
			// El siguiente es un extra, para no tener que salir del sistema otra vez
       		{ WAIT_END, CompruebaComienzo, WAIT_START, ReiniciaJuego},
			{-1, NULL, -1, NULL },
    };

    fsm_t* torreta_fsm = fsm_new (WAIT_START, controlTorreta, &(sistema.torreta));

    next = millis();

    /* Bucle infinito */
    while (1) {
    	/* Ejecucion de las maquinas de estados */
        fsm_fire (player_fsm);
        //fsm_fire (columns_fsm);
        //fsm_fire (keypad_fsm);
        fsm_fire (torreta_fsm);

        /* Delay CLK_MS milisegundos */
        next += CLK_MS;
        delay_until (next);
    }
    return 0;
}


/**************************************************************************/
