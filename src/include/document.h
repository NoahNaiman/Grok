#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "piece_chain.h"

/* 
 * Document Definition
 * ----------------------
 * Summary:
 */

typedef struct{
	int height;
	int width;
	char* text;
	char *lineAbove;
	char *lineBelow;
} Document_t;

/*
 * get_line_count Usage
 * --------------------------------
 * Get the number of lines in a given text file.
 *
 * Example:
 *	int lineCount = get_line_count(aFileName);
 *	-aFileName = an existing file's name
 */
int get_line_count(char* fileName);

/*
 * get_first_offscreen Usage
 * --------------------------------
 * Get starting of the first offscreen
 *
 * Example:
 *	int indexOffscreenLine = get_first_offscreen(textArray, linesInFile, height, width);
 *	-textArray = the text of a file
 *	-linesInFile = the number of 'actual' lines in text file
 *	-height = the height of the current ncurses window being printed to
 *	-width = the width of the current ncurses window being printed to
 */
int get_text_index(char* text, int linesInFile, int height, int width)