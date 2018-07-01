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

	FILE *fileDescriptor = fopen(fileName, "rb");

	if(fileDescriptor == NULL){

		newChain->original = (char *)malloc(BUFFERSIZE * sizeof(char));
		
		newChain->add = (char *)malloc(BUFFERSIZE * sizeof(char));

		newChain->pieces = init_splay_tree(0, 0, 0, 0);
	}
	else{
		int fileLength = get_original_size(fileDescriptor);
		newChain->original = (char *)malloc(fileLength * sizeof(char));

		size_t lengthRead = fread(newChain->original, sizeof(char), fileLength, fileDescriptor);
		newChain->original[lengthRead++] = '\n';
		newChain->original[lengthRead++] = '\0';

		fclose(fileDescriptor);

		newChain->add = (char *)malloc(fileLength * sizeof(char));

		newChain->pieces = init_splay_tree(0, 0, 0, lengthRead);
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
 *	SpkayTree_t* root
 *		-A pointer to a SplayTree_t
 *		-May be NULL
 */
//TODO: MODIFY TO STORE OUTPUT?
void print_chain(PieceChain_t* chain, SplayTree_t* root){
	if(root == NULL){
		return;
	}
	print_chain(chain, root->left);
	if(root->buffer == 0){
		printf("%.*s", root->length, &chain->original[root->physicalStart]);
	}
	else{
		printf("%.*s", root->length, &chain->add[root->physicalStart]);
	}
	print_chain(chain, root->right);
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
 *	integer
 *		-The length of the given file in bytes
 */
int get_original_size(FILE* fileDescriptor){
	fseek(fileDescriptor, 0, SEEK_END);
	int fileLength = ftell(fileDescriptor);
	rewind(fileDescriptor);
	return fileLength;
}

/*
 * get_current_length Function Definition
 * --------------------------------
 * Function Summary:
 *	Get the size in bytes of the current text
 *
 * Parameters:
 *	SplayTree_t* root
 *		-A pointer to the SplayTree_t recording info for
 *		 the current text
 *		-May not be NULL
 *
 * Return Type:
 *	integer
 *		-The current length of the text being edited
 */
int get_current_length(SplayTree_t* root){
	if(root == NULL){
		return 0;
	}
	int currentLength = 0;
	currentLength += root->length + get_current_length(root->left) + get_current_length(root->right);
	
	return currentLength;
}

/*
 * get_logical_start Function Definition
 * --------------------------------
 * Function Summary:
 *	Gets the logical start for a span of text from
 *	a given physical start location
 *
 * Parameters:
 *	SplayTree_t* root
 *		-A pointer to the SplayTree_t holding info of
 *		 a wanted span
 *		-May not be NULL
 *	int index
 *		-An integer representing a physical start index
 *		 to be translated into a logical one
 *
 * Return Type:
 *	integer
 *		-The logical start index equivalent to the given
 *		 physical one
 */
int get_logical_start(SplayTree_t* root, int index){
	if(root == NULL){
		return -1;
	}
	else if(index >= root->physicalStart && root <= (root->physicalStart + root->length)){
		return root->logicalStart + (index - root->physicalStart);
	}
	else if(index < root->physicalStart){
		return get_logical_start(root->left);
	}
	else{
		return get_logical_start(root->left);
	}
}

/*
 * get_physical_start Function Definition
 * --------------------------------
 * Function Summary:
 *	Gets the physical start for a span of text from
 *	a given logical start location
 *
 * Parameters:
 *	SplayTree_t* root
 *		-A pointer to the SplayTree_t holding info of
 *		 a wanted span
 *		-May not be NULL
 *	int index
 *		-An integer representing a logical start index
 *		 to be translated into a physical one
 *
 * Return Type:
 *	integer
 *		-The physical start index equivalent to the given
 *		 logical one
 */
int get_physical_start(SplayTree_t* root, int index){
	if(root == NULL){
		return -1;
	}
	else if(index >= root->logicalStart && root <= (root->logicalStart + root->length)){
		return root->physicalStart + (index - root->logicalStart);
	}
	else if(index < root->logicalStart){
		return get_physical_start(root->left);
	}
	else{
		return get_physical_start(root->left);
	}
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
 *		-An integer representing the physical start point of
 *		 recorded piece
 *	int length
 *		-An integer representing the recorded piece's length
 */
void record_piece(PieceChain_t* chain, int whichBuffer, int start, int length){
	int logicalStart = get_logical_start(chain->pieces, start);
	SplayTree_t* newNode = init_splay_tree(whichBuffer, start, logicalStart, length);
	chain->pieces = insert(chain->pieces, newNode);
}

int main() {
	PieceChain_t* myChain = init_piece_chain("src/splay_tree.c");
	print_chain(myChain, myChain->pieces);
	return 1;
}

