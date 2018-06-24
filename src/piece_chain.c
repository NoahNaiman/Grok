#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#include "include/piece_chain.h"

#ifndef BUFFERSIZE
#define BUFFERSIZE 494848
#endif


/* CORE FUNCTIONS */

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

/* UTILITY FUNCTIONS */

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

int get_physical_index(int logicalIndex){

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

int main() {
	PieceChain_t *myChain = init_piece_chain("obsolete/rope.c");
	print_chain(myChain);
	return 1;
}

