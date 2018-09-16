/******* TODOS: *******
 1. Add full document scrolling
 2. Add documentation
 3. Fixing scroll/type/cursor position difference issues
 **********************/

#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "include/piece_chain.h"

#define BUFFERSIZE 1024
#define KEY_DELETE 127
#define KEY_ESCAPE 27

WINDOW* init_grok(PieceChain_t *document){
	initscr();
	raw();
	noecho();
	WINDOW *newPad = newpad(LINES+1, COLS);
	nodelay(stdscr, true);
	halfdelay(25);
	keypad(stdscr, true);
	mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
	mouseinterval(0);
	wprintw(newPad, "%s\n", document->original);
	scrollok(newPad, true);
	wmove(newPad, 0, 0);
	prefresh(newPad, 0,0,0,0, LINES-1, COLS);
	return newPad;
}

void get_terminal_size(int *height, int *width){
	struct winsize window;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
    *height = window.ws_row;
    *width = window.ws_col;
}

//TODO: CHECK IF THERE IS ANYTHING ABOVE
void move_up(WINDOW *view, int *cursorY, int *cursorX, int *top, int *bottom){
	*cursorY -= -sizeof(char);
	wmove(view, *cursorY, *cursorX);
	prefresh(view, *top, 0, 0, 0, *bottom, COLS);
	if((int)(*cursorY-sizeof(char)) < 0){
		scrl(-1);
		move(0, *cursorX);
	}
	else{
		*cursorY -= sizeof(char);
		move(*cursorY, *cursorX);
	}
}

void move_down(WINDOW *view, int *cursorY, int *cursorX, int *top, int *bottom){
	int height;
	int width;
	getmaxyx(view, height, width);
	if((int)(*cursorY+sizeof(char)) >= height){
		//scroll(stdscr);
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
		if((int)(*cursorY+sizeof(char)) >= height){
			*cursorX = 0;
			scroll(stdscr);
			move(*cursorY, *cursorX);
			*cursorY = height;
		}
		else{
			*cursorY += sizeof(char);
			*cursorX = 0;
			move(*cursorY, *cursorX);
		}
	}
	else{
		*cursorX += sizeof(char);
		move(*cursorY, *cursorX);
	}
}

//TODO: FIX WEIRD WRAP-AROUND DELAY
void move_left(int *cursorY, int *cursorX){
	int height;
	int width;
	getmaxyx(stdscr, height, width);
	if((int)(*cursorX-sizeof(char)) < 0){
		if((int)*cursorY <= 0){
			*cursorX = width-1;
			*cursorY = 0;
			scrl(-1);
			move(0, *cursorX);
		}
		else{
			*cursorY -= sizeof(char);
			*cursorX = width-1;
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

void handle_input(WINDOW *view, int character, int *cursorX, int *cursorY, int *top, int *bottom, int *logicalStart, 
	int *pipelineIndex, char *pipelineBuffer, MEVENT event, PieceChain_t *document){
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
			move_up(view, cursorY, cursorX, top, bottom);
			break;
		case KEY_DOWN:
			move_down(view, cursorY, cursorX, top, bottom);
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
		default: //TODO: ADD TYPE OFF SCREEN PARAMETERS
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

	int cursorX = 0;
	int cursorY = 0;
	int height;
	int width;
	get_terminal_size(&height, &width);
	int top = 0;
	int bottom = height;
	int logicalStart = -1;
	int pipelineIndex = 0;
	char pipelineBuffer[BUFFERSIZE];

	WINDOW *view = init_grok(document);

	int currentChar;
	while((currentChar = wgetch(view)) != 'q'){
		handle_input(view, currentChar, &cursorX, &cursorY, &top, &bottom, &logicalStart, &pipelineIndex, pipelineBuffer, event, document);
	}
	endwin();


	return 0;
}