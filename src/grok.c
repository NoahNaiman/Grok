#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "include/piece_chain.h"

void enterRawMode(){
	struct termios raw;

	tcgetattr(STDIN_FILENO, &raw);
	raw.c_lflag &= ~(ECHO);

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(){
	printf("Welcome to grok!\n");
	enterRawMode();

	char currentChar;
	while(read(STDIN_FILENO, &currentChar, 1) == 1 && currentChar != 'q');
	return 0;
}