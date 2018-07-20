#ifndef PIECE_CHAIN_H
#define PIECE_CHAIN_H

#include "splay_tree.h"

/* 
 * Piece Chain Definition
 * ----------------------
 * Summary:
 *	Comprising of three seperate buffers. The first
 *	holds the original text, read in at launch. This
 *	buffer is read-only. The second holds any inserts
 *	made to the file. This is append only. Finally,
 *	the last is a binary tree of nodes containing
 *	which buffer the piece is in, its start index,
 *	and its length. This tree is optimized for
 *	localized operations by using a Splay Tree
 *
 * Typdef:
 *	PieceChain_t
 *
 * Values:
 *	char *original;
 *		-A buffer holding the original text
 *		-Read only
 *	char *add:
 *		-A buffer holding all additions to the
 *		 current file
 *		-Append only
 *	SplayTree_t *pieces:
 *		-A pointer to the root of a binary tree
 *		 wherein each node represents one piece
 *		-pieces->buffer holds which matrix is being used
 *			--0 if original
 *			--1 if add
 *		-pieces->physicalStart states the start index 
 *		 of the corresponding piece in its buffer
 *		-pieces->logicalStart states the start index of
 *		 the corresponding piece in the full text
 *		-pieces->length states the number of characters
 *		 in the corresponding span of text
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
	SplayTree_t *pieces;
} PieceChain_t;

/*
 * init_piece_chain Usage
 * --------------------------------
 * Initialize a PieceChain_t by reading in a given
 * file and assigning all necessary fields
 *
 * Example:
 *	PieceChain_t* aNewPieceChain = init_piece_chain(aFileName);
 *	-aFileName = an array of chars representing either an existing 
 *	 file or one to be created
 */
PieceChain_t* init_piecechain(char* fileName);

/*
 * record_piece Usage
 * --------------------------------
 * Record necessary information about a part of text
 * in the pieces buffer of a PieceChain_t
 *
 * Example:
 *	record_piece(anExistingPieceChain_t, originalOrAdd, startPoint, lengthOfPiece);
 *	-anExistingPieceChain_t = an already initialized PieceChain_t
 *	-originalOrAdd = 0 if original buffer, 1 if add buffer
 *	-startPoint = recorded piece's start index
 *	-lengthOfPiece = recorded piece's length
 */
void record_piece(PieceChain_t* chain, int whichBuffer, int start, int length);

/*
 * print_chain Usage
 * --------------------------------
 * Print some text as recorded by a given PieceChain_t
 *
 * Example:
 *	print_chain(anExistingPieceChain_t)
 *	-anExistingPieceChain_t = an already initialized PieceChain_t
 */
void print_chain(PieceChain_t* chain, SplayTree_t* root);

/*
 * get_original_size Usage
 * --------------------------------
 * Get the unedited size of a file
 *
 * Example:
 *	printf("The size of the file is: %d\n", get_original_size(fileDescriptor));
 *	-fileDescriptor = a non-NULL FILE pointer
 */
int get_original_size(FILE* fileDescriptor);

/*
 * get_current_size Usage
 * --------------------------------
 * Get the size of a file during or after edits
 *
 * Example:
 *	printf("The current size of the file is: %d\n", get_current_size(anExistingPieceChain_t));
 *	-anExistingPieceChain_t = an already initialized PieceChain_t
 */
int get_current_length(SplayTree_t* chain);

#endif