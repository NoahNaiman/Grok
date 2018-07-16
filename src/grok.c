#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "include/piece_chain.h"

struct termios originalTermios;

void enterRawMode(){
	tcgetattr(STDIN_FILENO, &originalTermios);
	atexit(exitRawMode);
	
	struct termios raw = originalTermios;
	raw.c_lflag &= ~(ECHO);

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void exitRawMode(){
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalTermios)
}

int main(){
	printf("Welcome to grok!\n");
	enterRawMode();

	char currentChar;
	while(read(STDIN_FILENO, &currentChar, 1) == 1 && currentChar != 'q');
	return 0;
}