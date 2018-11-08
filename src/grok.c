/******* TODOS: *******
 1. Add line number printing 
 2. Add documentation
 **********************/

#include <math.h>
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
	int lineCount = 2; //Unsure why this offest is necessary, but something to look into later.
	int lineLength = 1;
	char* currentPoint;
	for(currentPoint = text; *currentPoint; currentPoint++){
		if(*currentPoint == '\n'){
			lineLength = 1;
			lineCount++;
		}
		else if(lineLength > COLS){
			lineCount++;
		}
	}
	return(lineCount);
}

void print_with_lines(WINDOW* view, char* text, int lineCount){
	//Keep track of current line an its length
	int currentLine = 1;
	int currentLineLength = 0;

	//Keep track of how many spaces to print
	int emptySpacesToPrint;
	int emptySpaceCount = floor(log10(abs(lineCount)))+1;

	//Buffer to hold current line and pointer to overall text
	char lineBuffer[COLS-emptySpaceCount];
	char* ptr;
	for(ptr = text; *ptr; ptr++){
		if(*ptr == '\n' || currentLineLength >= (COLS-emptySpaceCount)){
			emptySpacesToPrint = emptySpaceCount - (floor(log10(abs(currentLine)))+1);
			while(emptySpacesToPrint != 0){
				wprintw(view, " ");
				emptySpacesToPrint--;
			}		
			wprintw(view, "%d %s\n", currentLine, lineBuffer);
			currentLine++;
			currentLineLength = 0;
			if(*(ptr+0x1) == '\n'){
				lineBuffer[0] = '\0';
			}
		}
		else{
			lineBuffer[currentLineLength] = *ptr;
			lineBuffer[currentLineLength+1] = '\0';
			currentLineLength++;
		}
	}
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
//	wprintw(newPad, "%s", fileText);
	print_with_lines(newPad, fileText, height);
	wmove(newPad, 0, 0);
	prefresh(newPad, 0,0,0,0, LINES-1, COLS);
	return(newPad);
}

void move_up(WINDOW *view, int *cursorY, int *cursorX, int *top, int *bottom){
	if(*cursorY != 0){
		if((int)(*cursorY-sizeof(char)) <= *top){
			*top -= 1;
			*bottom -= 1;
			*cursorY -= 1;
			wmove(view, *cursorY, *cursorX);
		}
		else{
			*cursorY -= 1;
			wmove(view, *cursorY, *cursorX);
		}
		prefresh(view, *top, 0, 0, 0, LINES-1, COLS);
	}
}

void move_down(WINDOW *view, int *cursorY, int *cursorX, int *top, int *bottom){
	if((int)(*cursorY+sizeof(char)) >= *bottom){
		*top += 1;
		*bottom += 1;
		*cursorY += 1;
		wmove(view, *cursorY, *cursorX);
	}
	else{
		*cursorY += 1;
		wmove(view, *cursorY, *cursorX);
	}
	prefresh(view, *top, 0, 0, 0, LINES-1, COLS);
}

void move_right(WINDOW *view, int *cursorY, int *cursorX, int *top, int *bottom){
	if((int)(*cursorX+sizeof(char)) >= COLS){
		if((int)(*cursorY+sizeof(char)) >= *bottom){
			*top += 1;
			*bottom += 1;
			*cursorY += 1;
			*cursorX = 0;	
		}
		else{
			*cursorY += 1; 
			*cursorX = 0;
		}
	}
	else{
		*cursorX += 1; 
	}
	wmove(view, *cursorY, *cursorX);
	prefresh(view, *top, 0, 0, 0, LINES-1, COLS);
}

void move_left(WINDOW *view, int *cursorY, int *cursorX, int *top, int *bottom){
	if((int)(*cursorX-sizeof(char)) < 0){
		if((int)*cursorY <= *top){
			*top -= 1;
			*bottom -= 1;
			*cursorY -= 1;
			*cursorX = COLS-1;
		}
		else{
			*cursorY -= 1;
			*cursorX = COLS-1; 
		}
	}
	else{
		*cursorX -= 1;
	}
	wmove(view, *cursorY, *cursorX);
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
			wdelch(view);
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
			move_right(view, cursorY, cursorX, top, bottom);
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
