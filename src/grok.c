#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "include/grok.h"

#define BUFFERSIZE 1024
#define KEY_DELETE 127
#define KEY_ESCAPE 27

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

Grokker_t* init_grokker(){
	Grokker_t* newGrokker = (Grokker_t *)malloc(sizeof(Grokker_t));
	newGrokker->cursorX = 0;
	newGrokker->cursorY = 0;
	getmaxyx(stdscr, newGrokker->height, newGrokker->width);
	newGrokker->logicalStart = -1;
	newGrokker->pipelineIndex = 0;
	newGrokker->pipelineToChain = (char *)malloc(BUFFERSIZE * sizeof(char));
	return newGrokker;
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

void handle_input(int character, Grokker_t *mainControl, MEVENT event, PieceChain_t *document){
	switch(character){
		case ERR:
			if(pipelineBuffer[0] != '\0'){
				int stringLength = strlen(pipelineBuffer);
				memcpy(document->add, pipelineBuffer, stringLength);
				record_piece(document, 1, *logicalStart, stringLength);
				*logicalStart = -1;
				memset(pipelineBuffer, '\0', BUFFERSIZE);
				printw("pipelineBuffer[0]: %s", pipelineBuffer[0]);
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
			printw("Hello");
			pipelineBuffer[*pipelineIndex] = character;
			printw("Before: %u", *pipelineIndex);
			*pipelineIndex += sizeof(char);
			printw("After: %u", *pipelineIndex);
			refresh();
	}
}

int main(int argc, char **argv){
	char *fileName  = argv[1];
	PieceChain_t *document = init_piecechain(fileName);
	MEVENT event;

	grok_init(document);
	Grokker_t* mainControl = init_grokker();

	int *logicalStart = mainControl->logicalStart;
	int *pipelineIndex = mainControl->pipelineIndex;
	int *cursorX = mainControl->cursorX;
	int *cursorY = mainControl->cursorY;
	int *height = mainControl->height;
	int *width = mainControl->width;

	int currentChar;
	while((currentChar = getch()) != 'q'){
		if(logicalStart != -1){
			logicalStart = ((*cursorY) * (*width))-((*width)-(*cursorX));
			pipelineIndex = 0;
		}
		printw("%d\n", logicalStart);
		printw("%d\n", pipelineIndex);
		printw("%d\n", cursorX);
		printw("%d\n", cursorY);
	}
	endwin();


	return 0;
}