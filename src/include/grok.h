#ifndef GROK
#define GROK
#endif

#include "piece_chain.h"

/* 
 * Grokker Definition
 * ----------------------
 * Summary:
 *	Containing all necessary informaton for Grok
 *	text-editor to function efficiently, including
 *	current cursor x and y, current window size, and
 *	ouput control variables. As an added bonus, allows
 *	all necesssary information to be easily accessed
 *	without the need for massive function calls.
 *
 * Typdef:
 *	Grokker_t
 *
 * Values:
 *	int cursorX;
 *		-The cursor's current x value in the window
 *	int cursorY;
 *		-The cursor's current y value in the window
 *	int height;
 *		-The window's current height
 *	int width;
 *		-The window's current width
 *	int logicalStart;
 *		-The logical start index of the current
 *		 span of text to be recorded into the 
 *		 program's piece chain
 *	int pipelineIndex:
 *		-Location to write next character to in
 *		 pipelineToChain buffer
 *	char* pipelineToChain
 *		-A buffer to store the next spans of text being
 *		 recorded in piece chain
 */
typedef struct{
	int cursorX;
	int cursorY;
	int height;
	int width;
	int logicalStart;
	int pipelineIndex;
	char *pipelineBuffer;
} Grokker_t;