#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include "include/grok.h"

#define CTRL_KEY(key) ((key) & 0x1f)

TerminalConfiguration_t configuration;


/* TERMINAL SETTING CONTROLS */
void die(const char *errorContextString){
	clear_screen();

	perror(errorContextString);
	exit(1);
}

void exit_raw_mode(){
	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &configuration.originalTermios) == -1){
		die("tcsetattr");
	}
}

void enter_raw_mode(){
	if(tcgetattr(STDIN_FILENO, &configuration.originalTermios) == -1){
		die("tcgetattr");
	}
	atexit(exit_raw_mode);

	struct termios raw = configuration.originalTermios;
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
int get_window_size(int *rows, int *columns){
	struct winsize windowSize;

	if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowSize) == -1 || windowSize.ws_col == 0){
		return -1;
	}
	else{
		*rows = windowSize.ws_row;
		*columns = windowSize.ws_col;
		return 0;
	}
}

char read_key_press(){
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

void process_keypress(){
	char currentChar = read_key_press();
	switch(currentChar){
		case CTRL_KEY('q'):
			clear_screen();
			exit(0);
			break;
	}
}

/* OUTPUT PROCESSING */

void draw_rows(){
	int y;
	for(y = 0; y < configuration.screenRows; y++){
		write(STDOUT_FILENO, "~\r\n", 3);
	}
}

void clear_screen(){
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);

	draw_rows();
	write(STDOUT_FILENO, "\x1b[H", 3);
}

/* INITIALIZER */
void init_editor(){
	if(get_window_size(&configuration.screenRows, &configuration.screenColumns) == -1){
		die("get_window_size");
	}
}

/* RUN PROGRAM */

int main(){
	printf("Welcome to grok!\n");
	enter_raw_mode();
	init_editor();

	while(1){
		clear_screen();
		process_keypress();
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