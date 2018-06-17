#include<stdlib.h>
#include<stdio.h>

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
 *	node
 *
 * Values:
 *	char string[]
 *		-Leaf: Characters representing some portion of text
 *		-Non-leaf: NULL
 *
 *	node left
 *		-A pointer to this node's left child node
 *
 *	node right
 *		-A pointer to this node's right child node
 */

typedef struct Rope node;

struct node {
	char *string;
	node *left;
	node *right;
};

int main(){
	return 1;
}
