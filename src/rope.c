#include<string.h>
#include<stdio.h>
#include<stdlib.h>

/*
 * Rope Definition
 * ---------------
 * Summary:
 *	A special case of a binary tree wherein each leaf
 *	contain a string. The weight of each node is the
 * 	length of its string if a leaf, and the total weight
 *	of all leaves in its left subtree if not.
 *
 * Typdef:
 *	ropeNode
 *
 * Values:
 *	char string[]
 *		-Leaf: Characters representing some portion of text
 *		-Non-leaf: NULL
 *
 *	int weight
 *		-This node's weight as defined above
 *
 *	ropeNode left
 *		-A pointer to this node's left child node
 *
 *	ropeNode right
 *		-A pointer to this node's right child node
 */

typedef struct Rope ropeNode;

struct ropeNode {
	char *string;
	int weight;
	ropeNode *left;
	ropeNode *right;
};

struct ropeNode* makeRopeNode(char* words){

	//Allocate space for new ropeNode
	struct ropeNode* newNode = (struct ropeNode*)malloc(sizeof(struct ropeNode));
	
	//Assign given words to newNode
	newNode->string = words;

	//Assign weight to newNode, zero if words is NULL
	newNode->weight = !words ? strlen(words) : 0;

	//Set newNode's children as NULL
	newNode->left = NULL;
	newNode->right = NULL;

	//Return newly constructed
	return(newNode);
}

int main(){
	return 1;
}
