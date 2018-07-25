#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "include/grok.h"

#define BUFFERSIZE 1024
#define KEY_DELETE 127

void grok_init(PieceChain_t *document){
	initscr();
	raw();
	noecho();
	nodelay(stdscr, true);
	halfdelay(15);
	keypad(stdscr, true);
	mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
	mouseinterval(0);
	print_chain(document, document->pieces);
	move(0, 0);
	scrollok(stdscr, true);
	refresh();
}

void move_up(y, x){
	getyx(stdscr, y, x);
	move(y-1, x);
}

void move_down(y, x){
	getyx(stdscr, y, x);
	move(y+1, x);
}

void move_right(y, x){
	getyx(stdscr, y, x);
	move(y, x+1);
}

void move_left(y, x){
	getyx(stdscr, y, x);
	move(y, x-1);
}

void delete(y, x){
	move_left(y, x);
	delch();
}

void handle_input(int character, int y, int x, int *writeFromIndex, MEVENT event, char *pipelineBuffer, PieceChain_t *document){
	switch(character){
		case ERR:
			if(pipelineBuffer[0] != 0){
				int stringLength = strlen(pipelineBuffer);
				memcpy(document->add, pipelineBuffer, stringLength);
				record_piece(document, 1, *writeFromIndex, stringLength);
				*writeFromIndex = -1;
				memset(pipelineBuffer, 0, BUFFERSIZE);
			}
			break;
		case KEY_BACKSPACE:
		case KEY_DC:
		case KEY_DELETE:
			delete(y, x);
			refresh();
			break;
		case KEY_UP:
			move_up(y, x);
			break;
		case KEY_DOWN:
			move_down(y, x);
			break;
		case KEY_RIGHT:
			move_right(y, x);
			break;
		case KEY_LEFT:
			move_left(y, x);
			break;
		case KEY_MOUSE:
			if(getmouse(&event) == OK){
				x = event.x;
				y = event.y;
				move(y, x);
				refresh();
			}
			break;
		default:
			printw("%c", character);
			refresh();
	}
}

int main(int argc, char **argv){
	char *fileName  = argv[1];
	PieceChain_t *document = init_piecechain(fileName);
	int x = 0;
	int y = 0;
	int height;
	int width;
	getmaxyx(stdscr, height, width);
	int writeFromIndex = -1;
	int pipelineIndex = 0;
	char pipelineBuffer[BUFFERSIZE];
	MEVENT event;

	grok_init(document);
	int currentChar;
	while((currentChar = getch()) != 'q'){
		if(writeFromIndex != -1){
			writeFromIndex = (y*width)-(width-x);
			pipelineIndex = 0;
		}
		handle_input(currentChar, y, x, &writeFromIndex, event, pipelineBuffer, document);
		pipelineBuffer[pipelineIndex] = currentChar;
		pipelineIndex++;
	}
	endwin();


	return 0;
}