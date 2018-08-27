#ifndef DOCUMENT_H
#define DOCUMENT_H
#endif

#include "piece_chain.h"

/* 
 * Document Definition
 * ----------------------
 * Summary:
 *	Solving the problem of how to display a
 *	long array of chars when working with
 *	terminal interface libraries such as ncurses.
 *	The document is comprised of three dynamically
 *	allocated char buffers which represent the
 *	onscreen text, the previous line for scrolling
 *	up, and the next line for scrolling down. As
 *	well, the height (rows) and width (columns) of
 *	the current terminal window are recorded.
 *
 * Typdef:
 *	Document_t
 *
 * Values:
 *	char height:
 *		-The height (rows) of the current
 *		 terminal window
 *	char width:
 *		-The width (columns) of the current
 *		 terminal window
 *	char *onscreenText:
 *		-A buffer holding the text currently
 *		 displayed in the terminal window
 *	char *lineAbove:
 *		-A buffer holding the previous line
 *		 which can be directly scrolled up to
 * 	char *lineBelow:
 *		-A buffer holding the next line
 *		 which can be directly scrolled down to
 *
 * Visual Example:
 *	lineAbove:
 *
 *	 1  Lorem ipsum I don't know the rest of
 *	
 *	onscreenText:
 *
 *	-------------------------------------------
 *	|2  the words but thank you very much for |
 *	|3  using/looking into Grok. This project |
 *	|4  was very fun for me to work on. I'm   |
 *	|5  sure many of you out there have looked|
 *	|6  at it and gone, "There are vim plugins|
 *	|7  that do all of this..." and sure there|
 *	|8  are but where is the fun in that, you |
 *	|9  know? I mean after developing this I  |
 *	|10 actually understand how those systems |
 *	|11 work more of less, and that knowledge |
 *	|12 truly is power. And if you don't feel |
 *	|13 the same then I completely understand,|
 *	-------------------------------------------	
 *
 *	lineBelow:
 *
 *	 14 either way it has been fun for me.	
 */

typedef struct{
	int height;
	int width;
	char *onscreenText;
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
int get_text_index(char* text, int linesInFile, int height, int width);