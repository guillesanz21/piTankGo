/** File Name		: lcdpiTankGo.c
 *  Description		: Implementación de una pantalla LCD para mostrar la
 *  información relevante del juego y del sistema.
 */


/* Includes -----------------------------------------------------------------*/
#include "lcdpiTankGo.h"

/* Variables -----------------------------------------------------*/
int impactos;
int disparos;
int lcdControl2;

/*
 * strobe:
 *	Toggle the strobe (Really the "E") pin to the device.
 *	According to the docs, data is latched on the falling edge.
 */
static void strobe (const struct lcdDataStruct *lcd)
{

// Note timing changes for new version of delayMicroseconds ()

  digitalWrite (lcd->strbPin, 1) ; delayMicroseconds (50) ;
  digitalWrite (lcd->strbPin, 0) ; delayMicroseconds (50) ;
}


/*
 * sentDataCmd:
 *	Send an data or command byte to the display.
 */
static void sendDataCmd (const struct lcdDataStruct *lcd, unsigned char data)
{
  register unsigned char myData = data ;
  unsigned char          i, d4 ;

  if (lcd->bits == 4)
  {
    d4 = (myData >> 4) & 0x0F;
    for (i = 0 ; i < 4 ; ++i)
    {
      digitalWrite (lcd->dataPins [i], (d4 & 1)) ;
      d4 >>= 1 ;
    }
    strobe (lcd) ;

    d4 = myData & 0x0F ;
    for (i = 0 ; i < 4 ; ++i)
    {
      digitalWrite (lcd->dataPins [i], (d4 & 1)) ;
      d4 >>= 1 ;
    }
  }
  else
  {
    for (i = 0 ; i < 8 ; ++i)
    {
      digitalWrite (lcd->dataPins [i], (myData & 1)) ;
      myData >>= 1 ;
    }
  }
  strobe (lcd) ;
}


/*
 * putCommand:
 *	Send a command byte to the display
 */
static void putCommand (const struct lcdDataStruct *lcd, unsigned char command)
{
  digitalWrite (lcd->rsPin,   0) ;
  sendDataCmd  (lcd, command) ;
  delay (2) ;
}

static void put4Command (const struct lcdDataStruct *lcd, unsigned char command)
{
  register unsigned char myCommand = command ;
  register unsigned char i ;

  digitalWrite (lcd->rsPin,   0) ;

  for (i = 0 ; i < 4 ; ++i)
  {
    digitalWrite (lcd->dataPins [i], (myCommand & 1)) ;
    myCommand >>= 1 ;
  }
  strobe (lcd) ;
}

/*
 *********************************************************************************
 * User Callable code below here
 *********************************************************************************
 */

/*
 * lcdHome: lcdClear:
 *	Home the cursor or clear the screen.
 */
void lcdHome (const int fd)
{
  struct lcdDataStruct *lcd = lcds [fd] ;

  putCommand (lcd, LCD_HOME) ;
  lcd->cx = lcd->cy = 0 ;
  delay (5) ;
}

void lcdClear (const int fd)
{
  struct lcdDataStruct *lcd = lcds [fd] ;

  putCommand (lcd, LCD_CLEAR) ;
  putCommand (lcd, LCD_HOME) ;
  lcd->cx = lcd->cy = 0 ;
  delay (5) ;
}


/*
 * lcdDisplay: lcdCursor: lcdCursorBlink:
 *	Turn the display, cursor, cursor blinking on/off
 */
void lcdDisplay (const int fd, int state)
{
  struct lcdDataStruct *lcd = lcds [fd] ;

  if (state)
    lcdControl |=  LCD_DISPLAY_CTRL ;
  else
    lcdControl &= ~LCD_DISPLAY_CTRL ;

  putCommand (lcd, LCD_CTRL | lcdControl) ;
}

void lcdCursor (const int fd, int state)
{
  struct lcdDataStruct *lcd = lcds [fd] ;

  if (state)
    lcdControl |=  LCD_CURSOR_CTRL ;
  else
    lcdControl &= ~LCD_CURSOR_CTRL ;

  putCommand (lcd, LCD_CTRL | lcdControl) ;
}

void lcdCursorBlink (const int fd, int state)
{
  struct lcdDataStruct *lcd = lcds [fd] ;

  if (state)
    lcdControl |=  LCD_BLINK_CTRL ;
  else
    lcdControl &= ~LCD_BLINK_CTRL ;

  putCommand (lcd, LCD_CTRL | lcdControl) ;
}


/*
 * lcdSendCommand:
 *	Send any arbitary command to the display
 */
void lcdSendCommand (const int fd, unsigned char command)
{
  struct lcdDataStruct *lcd = lcds [fd] ;
  putCommand (lcd, command) ;
}


/*
 * lcdPosition:
 *	Update the position of the cursor on the display.
 *	Ignore invalid locations.
 */
void lcdPosition (const int fd, int x, int y)
{
  struct lcdDataStruct *lcd = lcds [fd] ;

  if ((x > lcd->cols) || (x < 0))
    return ;
  if ((y > lcd->rows) || (y < 0))
    return ;

  putCommand (lcd, x + (LCD_DGRAM | rowOff [y])) ;

  lcd->cx = x ;
  lcd->cy = y ;
}


/*
 * lcdCharDef:
 *	Defines a new character in the CGRAM
 */
void lcdCharDef (const int fd, int index, unsigned char data [8])
{
  struct lcdDataStruct *lcd = lcds [fd] ;
  int i ;

  putCommand (lcd, LCD_CGRAM | ((index & 7) << 3)) ;

  digitalWrite (lcd->rsPin, 1) ;
  for (i = 0 ; i < 8 ; ++i)
    sendDataCmd (lcd, data [i]) ;
}


/*
 * lcdPutchar:
 *	Send a data byte to be displayed on the display. We implement a very
 *	simple terminal here - with line wrapping, but no scrolling. Yet.
 */
void lcdPutchar (const int fd, unsigned char data)
{
  struct lcdDataStruct *lcd = lcds [fd] ;

  digitalWrite (lcd->rsPin, 1) ;
  sendDataCmd  (lcd, data) ;

  if (++lcd->cx == lcd->cols)
  {
    lcd->cx = 0 ;
    if (++lcd->cy == lcd->rows)
      lcd->cy = 0 ;

    putCommand (lcd, lcd->cx + (LCD_DGRAM | rowOff [lcd->cy])) ;
  }
}


/*
 * lcdPuts:
 *	Send a string to be displayed on the display
 */
void lcdPuts (const int fd, const char *string)
{
  while (*string)
    lcdPutchar (fd, *string++) ;
}


/*
 * lcdPrintf:
 *	Printf to an LCD display
 */
void lcdPrintf (const int fd, const char *message, ...)
{
  va_list argp ;
  char buffer [1024] ;

  va_start (argp, message) ;
    vsnprintf (buffer, 1023, message, argp) ;
  va_end (argp) ;

  lcdPuts (fd, buffer) ;
}


/*
 * lcdInit:
 *	Take a lot of parameters and initialise the LCD, and return a handle to
 *	that LCD, or -1 if any error.
 */
int lcdInit (const int rows, const int cols, const int bits,
	const int rs, const int strb,
	const int d0, const int d1, const int d2, const int d3, const int d4,
	const int d5, const int d6, const int d7)
{
  static int initialised = 0 ;

  unsigned char func ;
  int i ;
  int lcdFd = -1 ;
  struct lcdDataStruct *lcd ;

  if (initialised == 0)
  {
    initialised = 1 ;
    for (i = 0 ; i < MAX_LCDS ; ++i)
      lcds [i] = NULL ;
  }

// Simple sanity checks

  if (! ((bits == 4) || (bits == 8)))
    return -1 ;

  if ((rows < 0) || (rows > 20))
    return -1 ;

  if ((cols < 0) || (cols > 20))
    return -1 ;

// Create a new LCD:

  for (i = 0 ; i < MAX_LCDS ; ++i)
  {
    if (lcds [i] == NULL)
    {
      lcdFd = i ;
      break ;
    }
  }

  if (lcdFd == -1)
    return -1 ;

  lcd = (struct lcdDataStruct *)malloc (sizeof (struct lcdDataStruct)) ;
  if (lcd == NULL)
    return -1 ;

  lcd->rsPin   = rs ;
  lcd->strbPin = strb ;
  lcd->bits    = 8 ;		// For now - we'll set it properly later.
  lcd->rows    = rows ;
  lcd->cols    = cols ;
  lcd->cx      = 0 ;
  lcd->cy      = 0 ;

  lcd->dataPins [0] = d0 ;
  lcd->dataPins [1] = d1 ;
  lcd->dataPins [2] = d2 ;
  lcd->dataPins [3] = d3 ;
  lcd->dataPins [4] = d4 ;
  lcd->dataPins [5] = d5 ;
  lcd->dataPins [6] = d6 ;
  lcd->dataPins [7] = d7 ;

  lcds [lcdFd] = lcd ;

  digitalWrite (lcd->rsPin,   0) ; pinMode (lcd->rsPin,   OUTPUT) ;
  digitalWrite (lcd->strbPin, 0) ; pinMode (lcd->strbPin, OUTPUT) ;

  for (i = 0 ; i < bits ; ++i)
  {
    digitalWrite (lcd->dataPins [i], 0) ;
    pinMode      (lcd->dataPins [i], OUTPUT) ;
  }
  delay (35) ; // mS


// 4-bit mode?
//	OK. This is a PIG and it's not at all obvious from the documentation I had,
//	so I guess some others have worked through either with better documentation
//	or more trial and error... Anyway here goes:
//
//	It seems that the controller needs to see the FUNC command at least 3 times
//	consecutively - in 8-bit mode. If you're only using 8-bit mode, then it appears
//	that you can get away with one func-set, however I'd not rely on it...
//
//	So to set 4-bit mode, you need to send the commands one nibble at a time,
//	the same three times, but send the command to set it into 8-bit mode those
//	three times, then send a final 4th command to set it into 4-bit mode, and only
//	then can you flip the switch for the rest of the library to work in 4-bit
//	mode which sends the commands as 2 x 4-bit values.

  if (bits == 4)
  {
    func = LCD_FUNC | LCD_FUNC_DL ;			// Set 8-bit mode 3 times
    put4Command (lcd, func >> 4) ; delay (35) ;
    put4Command (lcd, func >> 4) ; delay (35) ;
    put4Command (lcd, func >> 4) ; delay (35) ;
    func = LCD_FUNC ;					// 4th set: 4-bit mode
    put4Command (lcd, func >> 4) ; delay (35) ;
    lcd->bits = 4 ;
  }
  else
  {
    func = LCD_FUNC | LCD_FUNC_DL ;
    putCommand  (lcd, func     ) ; delay (35) ;
    putCommand  (lcd, func     ) ; delay (35) ;
    putCommand  (lcd, func     ) ; delay (35) ;
  }

  if (lcd->rows > 1)
  {
    func |= LCD_FUNC_N ;
    putCommand (lcd, func) ; delay (35) ;
  }

// Rest of the initialisation sequence

  lcdDisplay     (lcdFd, TRUE) ;
  lcdCursor      (lcdFd, FALSE) ;
  lcdCursorBlink (lcdFd, FALSE) ;
  lcdClear       (lcdFd) ;

  putCommand (lcd, LCD_ENTRY   | LCD_ENTRY_ID) ;
  putCommand (lcd, LCD_CDSHIFT | LCD_CDSHIFT_RL) ;

  return lcdFd ;
}

/*
 * @Brief		Método encargado de respresentar en la pantalla LCD la informaciion
 * 				correspondiente según el momento del programa.
 * @Note		Esta funcion usa los metodos proporcionados a traves de moodle
 */
int lcd(int mode, int lcdControl2){

	/* Usamos la variable "lcdControl" localmente, por lo que creamos
	una variable auxiliar para el resto de clases, "lcdControl2".  */
	lcdControl = lcdControl2;
	// Limpia la pantalla (LCD)
	lcdClear(lcdControl);

	if(mode == 1){
	    lcdPrintf(lcdControl, "Iniciando el    sistema...");
	    return 0;
	}
	if(mode == 2){
		lcdPrintf(lcdControl, "Juego           finalizado!");
		return 0;
	}
	if (mode == 3){
		lcdPrintf(lcdControl, "Se ha pausado el juego!");
		return 0;
	}

	/* ---------------------- Impactos --------------------- */
	/* Convierte el entero de impactos en "char" y lo copia. */
	char cimp = impactos + '0';
    char *str = "Imp: ";

    size_t len = strlen(str);
    char *str2 = malloc(len + 1 + 1 ); /* one for extra char, one for trailing zero */
    // Copia al string principal el numero de impactos
    strcpy(str2, str);
    str2[len] = cimp;
    str2[len + 1] = '\0';

    /* ---------------------- Disparos --------------------- */
	/* Convierte el entero de disparos en "char" y lo concatena. */
	char cdis = disparos + '0';
    char *str3 = " - Disp: ";

    size_t len2 = strlen(str3);
    char *str4 = malloc(len2 + len + 1 );  //one for extra char, one for trailing zero
    strcpy(str4, str3);
    str4[len2] = cdis;
    str4[len2 + 1] = '\0';

    strcat(str2,str4);

    /* ------------------------ Cancion ----------------------- */
    /* Concatena al string principal la informacion sobre la cancion de fondo*/
    strcat(str2,strCancion);

    /* ---------------------- Ultrasonidos --------------------- */
	/* Convierte el entero de la distancia del US en "char" y lo concatena.*/
    int US = USdistance;
    if (US > 99)
    	US = 0;
    char cUS[10];
	sprintf(cUS, "%d", US);
    char str5[18] = "US(cm): ";

    strcat(str5, cUS);
    strcat(str2,str5);

    lcdClear(lcdControl);
    lcdPrintf(lcdControl, str2);

	return 0;
}

/**************************************************************************/
