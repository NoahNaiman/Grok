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
	halfdelay(25);
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
	newGrokker->pipelineBuffer = (char *)malloc(BUFFERSIZE * sizeof(char));
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
			printw("index1: %d ", mainControl->pipelineIndex);
			if(mainControl->pipelineBuffer[0] != '\0'){
				char *pipelineBuffer = mainControl->pipelineBuffer;
				//printw("%s ", pipelineBuffer);
				/*printw("%d ", mainControl->index);
				int stringLength = strlen(mainControl->pipelineBuffer);
				memcpy(document->add, mainControl->pipelineBuffer, stringLength);
				record_piece(document, 1, *logicalStart, stringLength);
				*logicalStart = -1;
				memset(pipelineBuffer, '\0', BUFFERSIZE);*/
				printw("pipelineBuffer[0]: %c", mainControl->pipelineBuffer[0]);
			}
			break;
		case KEY_BACKSPACE:
		case KEY_DC:
		case KEY_DELETE:
			delete(mainControl->cursorY, mainControl->cursorX);
			refresh();
			break;
		case KEY_ESCAPE:
			printw("EXIT PRESSED");
		case KEY_UP:
			move_up(mainControl->cursorY, mainControl->cursorX);
			break;
		case KEY_DOWN:
			move_down(mainControl->cursorY, mainControl->cursorX);
			break;
		case KEY_RIGHT:
			move_right(mainControl->cursorY, mainControl->cursorX);
			break;
		case KEY_LEFT:
			move_left(mainControl->cursorY, mainControl->cursorX);
			break;
		case KEY_MOUSE:
			if(getmouse(&event) == OK){
				mainControl->cursorX = event.x;
				mainControl->cursorY = event.y;
				move(mainControl->cursorY, mainControl->cursorX);
				refresh();
			}
			break;
		default:
			printw("%c", character);
			int *index = &mainControl->pipelineIndex;
			mainControl->pipelineBuffer[*index] = character;
			printw("def index: %d ", *index);
			*index += sizeof(char);
			printw("def index: %d ", *index);
			refresh();
	}
}

int main(int argc, char **argv){
	char *fileName  = argv[1];
	PieceChain_t *document = init_piecechain(fileName);
	MEVENT event;

	grok_init(document);
	Grokker_t* mainControl = init_grokker();

	int *logicalStart = &mainControl->logicalStart;
	int *pipelineIndex = &mainControl->pipelineIndex;
	int *cursorX = &mainControl->cursorX;
	int *cursorY = &mainControl->cursorY;
	int *height = &mainControl->height;
	int *width = &mainControl->width;

	int currentChar;
	while((currentChar = getch()) != 'q'){
		if(*logicalStart == -1){
			*logicalStart = ((*cursorY)*(*width))-((*width)-(*cursorX));
			*pipelineIndex = 0;
		}
		handle_input(currentChar, mainControl, event, document);
	}
	endwin();


	return 0;
}