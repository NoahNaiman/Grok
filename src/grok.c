#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "include/grok.h"

#define KEY_DELETE 127

PieceChain_t *document;
char *fileName;
int x = 0;
int y = 0;

void print_text(){
	print_chain(document, document->pieces);
}

void delete(){

}

void handle_input(int character){
	switch(character){
		case KEY_DELETE:
			printw("DELETE HIT!!!");
			refresh();
			break;
		default:
			printw("%c", character);
			refresh();
	}
}

int main(int argc, char **argv){
	fileName = argv[1];
	document = init_piecechain(fileName);
	initscr();
	noecho();
	print_text();
	move(0, 0);
	scrollok(stdscr, TRUE);
	refresh();
	int currentChar;
	while((currentChar = getch()) != 'q'){
		handle_input(currentChar);
	}
	endwin();


	return 0;
}