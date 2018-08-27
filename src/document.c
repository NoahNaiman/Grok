#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/document.h"

/* INITIALIZER */

Document_t* init_document(PieceChain_t* chain, char* filename, int height, int width){
	Document_t* newDocument = (Document_t *)malloc(sizeof(Document_t));
	int lineCount = get_line_count(filename);
	if(lineCount > height){
		int lineAt = 0;
		
	}
	char text[height][width];
	get_first_offscreen(newDocument->lineBelow, chain->)
	return newDocument;
}

/* UTILITY FUNCTIONS */

int get_line_count(char *filename){
	FILE *filePointer = fopen(filename, "r");
	int lineCount = 0;
	char currentCharacter;

	for(currentCharacter = getc(filePointer); currentCharacter != EOF; currentCharacter = getc(filePointer)){
		if(currentCharacter == '\n'){
			lineCount += 1;
		}
	}
	fclose(filePointer);
	return(lineCount);
}

void get_first_offscreen(char* dest, char* text, int linesInFile, int height, int width){
	if(linesInFile <= height){
		dest = NULL;
	}
	else{
		char* currentCharacter;
		int index = 0;
		int lineCount = 0;
		int columnIndex = 0;
		for(currentCharacter = text; *currentCharacter != '\0'; currentCharacter += sizeof(char)){
			if(*currentCharacter == '\n' || columnIndex >= width){
				lineCount += 1;
				columnIndex = 0;
			}
			if(lineCount > height){
				if(*currentCharacter == '\n'){
					index += 1;
				}
				break;
			}
			index += 1;
		}
		char offscreen[width];
		char *offscreenPointer = offscreen;
		char *textPointer = &text[index];
		while(*textPointer != '\n' && *textPointer != '\0'){
			*offscreenPointer++ = *textPointer++;
		} 
		dest = offscreen;
	}
}