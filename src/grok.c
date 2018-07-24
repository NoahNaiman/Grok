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
	keypad(stdscr, TRUE);
	print_text();
	move(0, 0);
	scrollok(stdscr, TRUE);
	refresh();
}

void print_text(){
	print_chain(document, document->pieces);
}

void move_up(){
	getyx(stdscr, y, x);
	move(y-1, x);
}

void move_down(){
	getyx(stdscr, y, x);
	move(y+1, x);
}

void move_right(){
	getyx(stdscr, y, x);
	move(y, x+1);
}

void move_left(){
	getyx(stdscr, y, x);
	move(y, x-1);
}

void delete(){
	move_left();
	delch();
}

void handle_input(int character){
	switch(character){
		case KEY_BACKSPACE:
		case KEY_DC:
		case KEY_DELETE:
			delete();
			refresh();
			break;
		case KEY_UP:
			move_up();
			break;
		case KEY_DOWN:
			move_down();
			break;
		case KEY_RIGHT:
			move_right();
			break;
		case KEY_LEFT:
			move_left();
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