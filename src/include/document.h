#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "piece_chain.h"

/* 
 * Document Definition
 * ----------------------
 * Summary:
 */

typedef struct{
	PieceChain_t text;
	int rows;
	int columns;
	char *lineAbove;
	char *lineBelow;
} Document_t;