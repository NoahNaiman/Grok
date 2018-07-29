#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "include/grok.h"

#define BUFFERSIZE 1024
#define KEY_DELETE 127
#define KEY_ESCAPE 27

void init_grok(PieceChain_t *document){
	initscr();
	raw();
	noecho();
	nodelay(stdscr, true);
	halfdelay(25);
	keypad(stdscr, true);
	mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
	mouseinterval(0);
	print_chain(document, document->pieces);
	move(0, 0);
	scrollok(stdscr, true);
	refresh();
}

void move_up(cursorY, cursorX){
	getyx(stdscr, cursorY, cursorX);
	move(cursorY-1, cursorX);
}

void move_down(cursorY, cursorX){
	getyx(stdscr, cursorY, cursorX);
	move(cursorY+1, cursorX);
}

void move_right(cursorY, cursorX){
	getyx(stdscr, cursorY, cursorX);
	move(cursorY, cursorX+1);
}

void move_left(cursorY, cursorX){
	getyx(stdscr, cursorY, cursorX);
	move(cursorY, cursorX-1);
}

void delete(cursorY, cursorX){
	move_left(cursorY, cursorX);
	delch();
}

void handle_input(int character, int cursorX, int cursorY, int logicalStart, int *pipelineIndex, char *pipelineBuffer, MEVENT event, PieceChain_t *document){
	switch(character){
		case ERR:
			if(pipelineBuffer[0] != '\0'){
				/*int stringLength = strlen(pipelineBuffer);
				memcpy(document->add, pipelineBuffer, stringLength);
				record_piece(document, 1, logicalStart, stringLength);
				logicalStart = -1;
				memset(pipelineBuffer, '\0', BUFFERSIZE);*/
				printw("pipelineBuffer[0]: %s", pipelineBuffer);
			}
			break;
		case KEY_BACKSPACE:
		case KEY_DC:
		case KEY_DELETE:
			delete(cursorY, cursorX);
			refresh();
			break;
		case KEY_ESCAPE:
			printw("EXIT PRESSED");
		case KEY_UP:
			move_up(cursorY, cursorX);
			break;
		case KEY_DOWN:
			move_down(cursorY, cursorX);
			break;
		case KEY_RIGHT:
			move_right(cursorY, cursorX);
			break;
		case KEY_LEFT:
			move_left(cursorY, cursorX);
			break;
		case KEY_MOUSE:
			if(getmouse(&event) == OK){
				cursorX = event.x;
				cursorY = event.y;
				move(cursorY, cursorX);
				refresh();
			}
			break;
		default:
			printw("%c", character);
			pipelineBuffer[*pipelineIndex] = character;
			printw("def index: %d ", *pipelineIndex);
			*pipelineIndex += sizeof(char);
			printw("def index: %d ", *pipelineIndex);
			refresh();
	}
}

int main(int argc, char **argv){
	char *fileName  = argv[1];
	PieceChain_t *document = init_piecechain(fileName);
	MEVENT event;

	init_grok(document);

	int cursorX = 0;
	int cursorY = 0;
	int height;
	int width;
	int logicalStart = -1;
	int pipelineIndex = 0;
	char pipelineBuffer[BUFFERSIZE];

	int currentChar;
	while((currentChar = getch()) != 'q'){
		if(logicalStart == -1){
			getmaxyx(stdscr, height, width);
			logicalStart = ((cursorY * width)-(width - cursorX));
			pipelineIndex = 0;
		}
		handle_input(currentChar, cursorX, cursorY, logicalStart, &pipelineIndex, pipelineBuffer, event, document);
	}
	endwin();


	return 0;
}