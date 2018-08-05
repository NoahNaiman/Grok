#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "include/piece_chain.h"

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
	//idlok(stdscr, true);
	printw("%s\n", document->original);
	scrollok(stdscr, true);
	move(0, 0);
	refresh();
}

//TODO: CHECK IF THERE IS ANYTHING ABOVE
void move_up(int *cursorY, int *cursorX){
	if((int)(*cursorY-sizeof(char)) < 0){
		scrl(-1);
		move(0, *cursorX);
	}
	else{
		*cursorY -= sizeof(char);
		move(*cursorY, *cursorX);
	}
}

void move_down(int *cursorY, int *cursorX){
	int height;
	int width;
	getmaxyx(stdscr, height, width);
	if((int)(*cursorY+sizeof(char)) >= height){
		scroll(stdscr);
		move(height, *cursorX);
	}
	else{
		*cursorY += sizeof(char);
		move(*cursorY, *cursorX);
	}
}

void move_right(int *cursorY, int *cursorX){
	int height;
	int width;
	getmaxyx(stdscr, height, width);
	if((int)(*cursorX+sizeof(char)) >= width){
		*cursorY += sizeof(char);
		*cursorX = 0;
		move(*cursorY, *cursorX);
	}
	else{
		*cursorX += sizeof(char);
		move(*cursorY, *cursorX);
	}
}

void move_left(int *cursorY, int *cursorX){
	int height;
	int width;
	getmaxyx(stdscr, height, width);
	if((int)(*cursorX-sizeof(char)) <= 0){
		if(*cursorY == 0){
			*cursorX = width;
			move(*cursorY, width);
			scrl(-1);
		}
		else{
			*cursorY -= sizeof(char);
			*cursorX = width;
			move(*cursorY, *cursorX);
		}
	}
	else{
		*cursorX -= sizeof(char);
		move(*cursorY, *cursorX);
	}
}

void delete(int *cursorY, int *cursorX){
	move_left(cursorY, cursorX);
	delch();
}

void handle_input(int character, int *cursorX, int *cursorY, int *logicalStart, int *pipelineIndex, char *pipelineBuffer, MEVENT event, PieceChain_t *document){
	switch(character){
		case ERR:
			if(pipelineBuffer[0] != '\0'){
				int stringLength = strlen(pipelineBuffer);
				memcpy(document->add, pipelineBuffer, stringLength);
				record_piece(document, 1, *logicalStart, stringLength);
				int height;
				int width;
				getmaxyx(stdscr, height, width);
				*logicalStart = ((*cursorY) * width) - (*cursorX);
				*pipelineIndex = 0;
				memset(pipelineBuffer, '\0', BUFFERSIZE);
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
				*cursorX = event.x;
				*cursorY = event.y;
				move(*cursorY, *cursorX);
				refresh();
			}
			break;
		default:
			if(*logicalStart == -1){
				int height;
				int width;
				getmaxyx(stdscr, height, width);
				*logicalStart = ((*cursorY * width)-(*cursorX));
			}
			printw("%c", character);
			pipelineBuffer[*pipelineIndex] = character;
			*pipelineIndex += sizeof(char);
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
	int logicalStart = -1;
	int pipelineIndex = 0;
	char pipelineBuffer[BUFFERSIZE];

	int currentChar;
	while((currentChar = getch()) != 'q'){
		handle_input(currentChar, &cursorX, &cursorY, &logicalStart, &pipelineIndex, pipelineBuffer, event, document);
	}
	endwin();


	return 0;
}