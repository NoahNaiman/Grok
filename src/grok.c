#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "include/grok.h"

#define CTRL_KEY(key) ((key) & 0x1f)

struct termios originalTermios;

/* TERMINAL SETTING CONTROLS */
void die(const char *errorContextString){
	clearScreen();

	perror(errorContextString);
	exit(1);
}

void exitRawMode(){
	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalTermios) == -1){
		die("tcsetattr");
	}
}

void enterRawMode(){
	if(tcgetattr(STDIN_FILENO, &originalTermios) == -1){
		die("tcgetattr");
	}
	atexit(exitRawMode);

	struct termios raw = originalTermios;
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;

	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1){
		die("tcsetattr");
	}
}

char readKeypress(){
	int newRead;
	char currentChar;
	while((newRead = read(STDIN_FILENO, &currentChar, 1)) != 1){
		if(newRead == -1 && errno != EAGAIN){
			die("read");
		}
	}
	return currentChar;
}

/* INPUT PROCESSING */

void processKeypress(){
	char currentChar = readKeypress();
	switch(currentChar){
		case CTRL_KEY('q'):
			clearScreen();
			exit(0);
			break;
	}
}

/* OUTPUT PROCESSING */
void clearScreen(){
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);
}

int main(){
	printf("Welcome to grok!\n");
	enterRawMode();

	while(1){
		clearScreen();
		processKeypress();
		// currentChar = '\0';
		// if(read(STDIN_FILENO, &currentChar, 1) == -1 && errno != EAGAIN){
		// 	die("read");
		// }
		// if(iscntrl(currentChar)){
		// 	printf("%d\n", currentChar);
		// }
		// else{
		// 	printf("%d ('%c')\r\n", currentChar, currentChar);
		// }
		// if(currentChar == CTRL_KEY('q')){
		// 	break;
		// }
	}
	
	return 0;
}