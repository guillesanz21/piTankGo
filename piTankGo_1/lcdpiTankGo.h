/** File Name		: lcdpiTankGo.h
*/

#ifndef LCD_H_
#define LCD_H_

/*
 * lcd.c:
 *	Text-based LCD driver.
 *	This is designed to drive the parallel interface LCD drivers
 *	based in the Hitachi HD44780U controller and compatables.
 *
 * Copyright (c) 2012 Gordon Henderson.
 ***********************************************************************
 * This file is part of wiringPi:
 *	https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */

/* Includes -----------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <wiringPi.h>

#include "lcd.h"
#include "piTankGolib.h"


/* Defines --------------------------------------------------------*/
#ifndef	TRUE
#  define	TRUE	(1==1)
#  define	FALSE	(1==2)
#endif

// HD44780U Commands

#define	LCD_CLEAR	0x01
#define	LCD_HOME	0x02
#define	LCD_ENTRY	0x04
#define	LCD_CTRL	0x08
#define	LCD_CDSHIFT	0x10
#define	LCD_FUNC	0x20
#define	LCD_CGRAM	0x40
#define	LCD_DGRAM	0x80

// Bits in the entry register

#define	LCD_ENTRY_SH		0x01
#define	LCD_ENTRY_ID		0x02

// Bits in the control register

#define	LCD_BLINK_CTRL		0x01
#define	LCD_CURSOR_CTRL		0x02
#define	LCD_DISPLAY_CTRL	0x04

// Bits in the function register

#define	LCD_FUNC_F	0x04
#define	LCD_FUNC_N	0x08
#define	LCD_FUNC_DL	0x10

#define	LCD_CDSHIFT_RL	0x04

/* Struct ---------------------------------------------------------*/
struct lcdDataStruct
{
  int bits, rows, cols ;
  int rsPin, strbPin ;
  int dataPins [8] ;
  int cx, cy ;
} ;

struct lcdDataStruct *lcds [MAX_LCDS] ;

// Row offsets

/* Variables --------------------------------------------------------*/
static const int rowOff [4] = { 0x00, 0x40, 0x14, 0x54 } ;
int lcdControl ;

/* Functions --------------------------------------------------------*/
int lcd(int mode, int lcdControl2);
void lcdHome (const int fd);
void lcdClear (const int fd);
void lcdDisplay (const int fd, int state);
void lcdCursor (const int fd, int state);
void lcdCursorBlink (const int fd, int state);
void lcdSendCommand (const int fd, unsigned char command);
void lcdPosition (const int fd, int x, int y);
void lcdCharDef (const int fd, int index, unsigned char data [8]);
void lcdPutchar (const int fd, unsigned char data);
void lcdPuts (const int fd, const char *string);
void lcdPrintf (const int fd, const char *message, ...);
int lcdInit (const int rows, const int cols, const int bits,
	const int rs, const int strb,
	const int d0, const int d1, const int d2, const int d3, const int d4,
	const int d5, const int d6, const int d7);


#endif /* LCD_H_ */


/**************************************************************************/
