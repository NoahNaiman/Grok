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

int get_cursor_position(int *rows, int *columns){
	char buffer[32];

	if(write(STDOUT_FILENO, "\x1b[6n", 4) != 4){
		return -1;
	}

	unsigned int i = 0;
	while(i < sizeof(buffer)-1){
		if(read(STDIN_FILENO, &buffer[i], 1) != 1){
			break;
		}
		if(buffer[i] == 'R'){
			break;
		}
		i++;
	}
	buffer[i] = '\0';

	if(buffer[0] != '\x1b' || buffer[1] != '['){
		return -1;
	}
	if(sscanf(&buffer[2], "%d;%d", rows, columns) != 2){
		return -1;
	}

	return 0;
}

int get_window_size(int *rows, int *columns){
	struct winsize windowSize;

	if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowSize) == -1 || windowSize.ws_col == 0){
		if(write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12){
			return -1;
		}
		return get_cursor_position(rows, columns);
	}
	else{
		*rows = windowSize.ws_row;
		*columns = windowSize.ws_col;
		return 0;
	}
}

char read_keypress(){
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
	char currentChar = read_keypress();
	switch(currentChar){
		case CTRL_KEY('q'):
			clear_screen();
			exit(0);
			break;
	}
}

/* OUTPUT PROCESSING */

void draw_rows(){
	int i;
	for(i = 0; i < configuration.screenRows; i++){
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