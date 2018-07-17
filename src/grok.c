#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "include/piece_chain.h"

struct termios originalTermios;

void exitRawMode(){
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalTermios);
}

void enterRawMode(){
	tcgetattr(STDIN_FILENO, &originalTermios);
	atexit(exitRawMode);

	struct termios raw = originalTermios;
	raw.c_iflag &= ~(IXON);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(){
	printf("Welcome to grok!\n");
	enterRawMode();

	char currentChar;
	while(read(STDIN_FILENO, &currentChar, 1) == 1 && currentChar != 'q'){
		if(iscntrl(currentChar)){
			printf("%d\n", currentChar);
		}
		else{
			printf("%d ('%c')\n", currentChar, currentChar);
		}
	}
	return 0;
}