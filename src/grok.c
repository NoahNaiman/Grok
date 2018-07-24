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

void grok_init(){
	initscr();
	noecho();
	print_text();
	move(0, 0);
	scrollok(stdscr, TRUE);
	refresh();
}

void print_text(){
	print_chain(document, document->pieces);
}

void move_left(){
	getyx(stdscr, y, x);
	move(y, x-1);
}

void move_right(){
	getyx(stdscr, y, x);
	move(y, x+1);
}

void move_up(){
	getyx(stdscr, y, x);
	move(y-1, x);
}

void move_down(){
	getyx(stdscr, y, x);
	move(y+1, x);
}

void delete(){
	move_left();
	delch();
}

void handle_input(int character){
	switch(character){
		case KEY_DELETE:
			delete();
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
	grok_init();
	int currentChar;
	while((currentChar = getch()) != 'q'){
		handle_input(currentChar);
	}
	endwin();


	return 0;
}