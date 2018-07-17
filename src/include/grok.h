#ifndef GROK
#define GROK
#endif

#include "piece_chain.h"

typedef struct{
	int screenRows;
	int screenColumns;
	struct termios originalTermios;
} TerminalConfiguration_t;

/*
 * clearScreen Usage
 * --------------------------------
 * Clears terminal screen and resets cursor to
 * top left.
 *
 * Example:
 *	if(readyToExitProgram == 1){
 *		clearScreen();	
 *	}
 */
void clear_screen();

char read_keypress();