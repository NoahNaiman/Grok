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

int get_num_lines(char* text){
	int lineCount = 0;
	char* currentPoint;
	for(currentPoint = text; *currentPoint != EOF; currentPoint++){
		if(*currentPoint == '\n'){
			lineCount++;
		}
	}
	return(lineCount);
}

WINDOW* init_grok(PieceChain_t *document){
	initscr();
	raw();
	noecho();
	char* fileText = document->original;
	int height = get_num_lines(fileText);
	WINDOW *newPad = newpad(height, COLS);
	nodelay(newPad, TRUE);
	halfdelay(25);
	keypad(newPad, TRUE);
	mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
	mouseinterval(0);
	scrollok(newPad, TRUE);
	idlok(newPad,TRUE);
	wprintw(newPad, "%s", fileText);
	wmove(newPad, 0, 0);
	prefresh(newPad, 0,0,0,0, LINES-1, COLS);
	return(newPad);
}

void get_terminal_size(int *height, int *width){
	struct winsize window;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
    *height = window.ws_row;
    *width = window.ws_col;
}

//TODO: CHECK IF THERE IS ANYTHING ABOVE
void move_up(WINDOW *view, int *cursorY, int *cursorX, int *top, int *bottom){
	if(/*(*top != 0) &&*/ (int)(*cursorY-sizeof(char)) < *top){
		*top -= 1;
		*bottom -= 1;
		wmove(view, *cursorY, *cursorX);
	}
	else{
		*cursorY -= 1;
		wmove(view, *cursorY, *cursorX);
	}
	prefresh(view, *top, 0, 0, 0, LINES-1, COLS);
}

void move_down(WINDOW *view, int *cursorY, int *cursorX, int *top, int *bottom){
	if((int)(*cursorY+sizeof(char)) >= *bottom){
		*top += 1;
		*bottom += 1;
		wmove(view, *cursorY, *cursorX);
	}
	else{
		*cursorY += 1;
		wmove(view, *cursorY, *cursorX);
	}
	prefresh(view, *top, 0, 0, 0, LINES-1, COLS);
}

void move_right(WINDOW *view, int *cursorY, int *cursorX, int *top, int *bottom){
	int height;
	int width;
	getmaxyx(view, height, width);
	if((int)(*cursorX+sizeof(char)) >= width){
		if((int)(*cursorY+sizeof(char)) >= height){
			*cursorX = 0;
			scroll(view);
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
	prefresh(view, *top, 0, 0, 0, LINES-1, COLS);
}

//TODO: FIX WEIRD WRAP-AROUND DELAY
void move_left(WINDOW *view, int *cursorY, int *cursorX, int *top, int *bottom){
	int height;
	int width;
	getmaxyx(view, height, width);
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
	prefresh(view, *top, 0, 0, 0, LINES-1, COLS);
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
				getmaxyx(view, height, width);
				*logicalStart = ((*cursorY) * width) - (*cursorX);
				*pipelineIndex = 0;
				memset(pipelineBuffer, '\0', BUFFERSIZE);
			}
			break;
		case KEY_BACKSPACE:
		case KEY_DC:
		case KEY_DELETE:
			move_left(view, cursorY, cursorX, top, bottom);
			delch();
			prefresh(view, *top, 0, 0, 0, LINES-1, COLS);
			break;
		case KEY_UP:
			move_up(view, cursorY, cursorX, top, bottom);
			break;
		case KEY_DOWN:
			move_down(view, cursorY, cursorX, top, bottom);
			break;
		case KEY_RIGHT:
			move_right(view, cursorY, cursorX, top, bottom);
			break;
		case KEY_LEFT:
			move_left(view, cursorY, cursorX, top, bottom);
			break;
		case KEY_MOUSE:
			if(getmouse(&event) == OK){
				*cursorX = event.x;
				*cursorY = event.y;
				wmove(view, *cursorY, *cursorX);
				prefresh(view, *top, 0, 0, 0, LINES-1, COLS);
			}
			break;
		case KEY_ESCAPE:
			wprintw(view, "EXIT PRESSED");
			prefresh(view, *top, 0, 0, 0, LINES-1, COLS);
			break;
		default: 
	/*		if(*logicalStart == -1){
				int height;
				int width;
				getmaxyx(view, height, width);
				*logicalStart = ((*cursorY * width)-(*cursorX));
			}*/
			wprintw(view, "%c", character);
		//	pipelineBuffer[*pipelineIndex] = character;
		//	*pipelineIndex += sizeof(char);
			prefresh(view, *top, 0, 0, 0, LINES-1, COLS);
	}
}

int main(int argc, char **argv){
	char *fileName  = argv[1];
	PieceChain_t *document = init_piecechain(fileName);
	MEVENT event;

	WINDOW *view = init_grok(document);	

	int cursorX = 0;
	int cursorY = 0;
	int top = 0;
	int bottom = LINES;
	int logicalStart = -1;
	int pipelineIndex = 0;
	char pipelineBuffer[BUFFERSIZE];
	prefresh(view, top, 0, 0, 0, bottom, COLS);	

	int currentChar;
	while((currentChar = wgetch(view)) != 'q'){
		handle_input(view, currentChar, &cursorX, &cursorY, &top, &bottom, &logicalStart, &pipelineIndex, pipelineBuffer, event, document);
	}
	endwin();


	return 0;
}
