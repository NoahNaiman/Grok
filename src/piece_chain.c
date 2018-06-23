#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#ifndef BUFFERSIZE
#define BUFFERSIZE 494848
#endif

/* 
 * Piece Chain Definition
 * ----------------------
 * Summary:
 *	Comprising of three seperate buffers. The first
 *	holds the original text, read in at launch. This
 *	buffer is read-only. The second holds any inserts
 *	made to the file. This is append only. Finally,
 *	the last is a table containing which buffer the
 *	piece is in, its start index, and its length.
 *
 * Typdef:
 *	piece_chain_t
 *
 * Values:
 *	char *original;
 *		-A buffer holding the original text
 *		-Read only
 *	char *add:
 *		-A buffer holding all additions to the
 *		 current file
 *		-Append only
 *	int *pieces:
 *		-A 1024 by 3 matrix where each row represnts
 *		 one piece.
 *		-pieces[n][0] holds which matrix is being used
 *			--0 if original
 *			--1 if add
 *		-pieces[n][1] states the start of the current piece
 *		-pieces[n][2] states the length of the current piece
 *
 *
 * Visual Example:
 *
 *				BEFORE ANY EDITS
 *				----------------
 *	
 *	Original file buffer (read only):
 *
 *	-----------------------------------------
 *	|A| |l|a|r|g|e| |s|p|a|n| |o|f| |t|e|x|t|
 *	-----------------------------------------
 *
 *	Add buffer (append only):
 *
 *	-----------------------------------------
 *	|										|
 *	-----------------------------------------
 *
 *	Piece Table:
 *
 *	BUFFER 	   |  START INDEX  | PIECE LENGTH
 *	------------------------------------------
 *	| original | 			 0 |		   19|
 *	------------------------------------------
 *
 *	Text:
 *
 *	A large span of text
 *	------------------------------------------
 *
 *				  AFTER EDITS
 *				----------------
 *	
 *	Original file buffer (read only):
 *
 *	-----------------------------------------
 *	|A| |l|a|r|g|e| |s|p|a|n| |o|f| |t|e|x|t|
 *	-----------------------------------------
 *
 *	Add buffer (append only):
 *
 *	-----------------------------------------
 *	|E|n|g|l|i|s|h|							|
 *	-----------------------------------------
 *
 *	Piece Table
 *
 *	BUFFER 	   |  START INDEX  | PIECE LENGTH
 *	-------------------------------------------
 *  | original | 			 0 |		    2 |
 *	| original |			 8 |		   	8 |
 *	| add  	   |			 0 |		    8 |
 *	| original |			16 |			4 |
 *  -------------------------------------------
 *
 * 	Text:
 *
 * 	A span of English text
 */
typedef struct{
	char *original;
	char *add;
	int **pieces;
} PieceChain_t;


/*
 * get_original_size Function Definition
 * --------------------------------
 * Function Summary:
 *	Get the size in bytes of a given text file
 *
 * Parameters:
 *	FILE* fileDescriptor
 *		-A pointer to a file
 *		-May not be NULL
 *
 * Return Type:
 *	int fileLength
 *		-The length of the given file in bytes
 */
int get_original_size(FILE* fileDescriptor){
	fseek(fileDescriptor, 0, SEEK_END);
	int fileLength = ftell(fileDescriptor);
	rewind(fileDescriptor);
	return fileLength;
}

/*
 * get_current_size Function Definition
 * --------------------------------
 * Function Summary:
 *	Get the size in bytes of the current text
 *
 * Parameters:
 *	PieceChain_t* chain
 *		-A pointer to the PieceChain_t recording info for
 *		 the current text
 *		-May not be NULL
 *
 * Return Type:
 *	int currentLength
 *		-The current length of the text being edited
 */
int get_current_length(PieceChain_t* chain){
	int currentLength = 0;
	int i = 0;
	for(; chain->pieces[i][2] != 0; i++){
		currentLength += chain->pieces[i][2];
	}
	return currentLength;
}

/*
 * record_piece Function Definition
 * --------------------------------
 * Function Summary:
 *	Records necessary information on a given piece in chain's
 *	pieces buffer
 *		-Records which buffer is being pointed to in pieces[n][0]
 *		-Records starting point of piece in pieces[n][1]
 *		-Records length of piece in pieces[n][2]
 *
 * Parameters:
 *	PieceChain_t* chain
 *		-A pointer to the PieceChain_t whose pieces buffer will
 *		 hold the recorded information
 *		-May not be NULL
 *	int whichBuffer
 *		-An integer representing buffer the recorded piece is in
 *		-0 if original
 *		-1 if add
 *	int start
 *		-An integer representing the start point of recorded piece
 *
 *	int whichBuffer
 *		-An integer representing the recorded piece's length
 */
void record_piece(PieceChain_t* chain, int whichBuffer, int start, int length){
	int i = 0;
	for(; chain->pieces[i][2] != 0; i++);

	chain->pieces[i][0] = whichBuffer;
	chain->pieces[i][1] = start;
	chain->pieces[i][2] = length;
}

PieceChain_t* init_piece_chain(char* fileName){

	PieceChain_t*  newChain = (PieceChain_t *)malloc(sizeof(PieceChain_t));

	int i;
	newChain->pieces = (int **)malloc(BUFFERSIZE * sizeof(int *));
	for(i=0; i < BUFFERSIZE; i++){
		newChain->pieces[i] = (int *)malloc(3 * sizeof(int));
	}
	
	FILE *fileDescriptor = fopen(fileName, "rb");

	if(fileDescriptor == NULL){

		newChain->original = (char *)malloc(BUFFERSIZE * sizeof(char));
		record_piece(newChain, 0, 0, 0);

		newChain->add = (char *)malloc(BUFFERSIZE * sizeof(char));
	}
	else{
		int fileLength = get_original_size(fileDescriptor);
		newChain->original = (char *)malloc(fileLength * sizeof(char));

		size_t lengthRead = fread(newChain->original, sizeof(char), fileLength, fileDescriptor);
		newChain->original[lengthRead++] = '\0';

		fclose(fileDescriptor);

		record_piece(newChain, 0, 0, lengthRead);

		newChain->add = (char *)malloc(fileLength * sizeof(char));
	}
	return newChain;
}

/*
 * print_chain Function Definition
 * --------------------------------
 * Function Summary:
 *	Print the recorded text to standard output
 *
 * Parameters:
 *	PieceChain_t* chain
 *		-A pointer to a PieceChain_t
 *		-May not be NULL
 */
//TODO: MODIFY TO STORE OUTPUT
void print_chain(PieceChain_t* chain){
	int i = 0;
	for(; chain->pieces[i][2] != 0; i++){
		if(chain->pieces[i][0] == 0){
			printf("%.*s", chain->pieces[i][2], &chain->original[chain->pieces[i][1]]);
		}
		else{
			printf("%.*s", chain->pieces[i][2], &chain->add[chain->pieces[i][1]]);
		}
	}
}

int main() {
	PieceChain_t *myChain = init_piece_chain("obsolete/rope.c");
	print_chain(myChain);
	return 1;
}

