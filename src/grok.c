#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "include/grok.h"

PieceChain_t *document;
char *fileName;

void print_text(){
	print_chain(document, document->pieces);
}

int main(int argc, char **argv){
	fileName = argv[1];
	document = init_piecechain(fileName);
	initscr();	
	print_text();
	refresh();
	getch();
	endwin();


	return 0;
}