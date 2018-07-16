#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/piece_chain.h"

#define ERROR_INSERTION 0x1
#define ERROR_DELETION 0x2
#define ERROR_TEXT_DIFFERENCE 0x3

void print_test_result(int code){
	if(code){
		printf("Failed due to: ");
		if(code & ERROR_INSERTION){
			printf("<INSERTION ERROR>");
		}
		else if(code & ERROR_DELETION){
			printf("<DELETION ERROR>");
		}
		else if(code & ERROR_TEXT_DIFFERENCE){
			printf("<TEXT FORMATTING FAILURE>");
		}
	}
	else{
		printf("Test passed!");
	}
}

int test_insertion(File* fileDescriptor, PieceChain_t* testChain){

}

int main(){
	return 1;
}