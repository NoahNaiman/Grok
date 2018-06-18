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

//Declare and typdef (alias) Rope as ropeNode
typedef struct Rope {
	char *string;
	int weight;
	struct Rope *left;
	struct Rope *right;
} ropeNode;


/*
 * makeRopeNode Function Definition
 * --------------------------------
 * Function Summary:
 *	Creates a new ropeNode
 *		-Sets *string to a given string of characters or NULL
 *		 if none is given
 *		-Sets weight to length of given string or zero if none
 *		 is given
 *		-Sets left child to NULL
 *		-Sets right child to NULL
 *
 * Parameters:
 *	char* words
 *		-A pointer to a string of characters
 *		-May be NULL
 *
 * Return Type:
 *	ropeNode*
 *		-A newly created rope node with no children
 */

ropeNode* makeRopeNode(char* words){

	//Allocate space for new ropeNode
	ropeNode* newNode = (ropeNode*)malloc(sizeof(ropeNode));
	
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

/*
 * concatenate Function Definition
 * --------------------------------
 * Function Summary:
 *	Concatenates a string by creating a new parent ropeNode
 *		-Sets newParent's left child to what should
 *		 be the former part of the new string
 *		-Sets newParent's right child to what should be the
 *		 latter part of the new string
 *		-Sets newParent's *string to NULL
 *		-Sets newParent's weight to the length of the string
 *		 in its left subtree
 *
 * Parameters:
 *	ropeNode* left
 *		-A pointer to a ropeNode whose string represents the
 *		 former part of some text
 *		-Should not be NULL as this could cause unintended
 *		 side effects
 *	ropeNode* right
 *		-A pointer to a ropeNode whose string represents the
 *		 latter part of some text
 *		-Should not be NULL as this could cause unintended
 *		 side effects
 *
 * Return Type:
 *	ropeNode*
 *		-A newly created rope node whose children represent
 *		 two parts of a newly concatenated string
 */

//TODO: SET WEIGHT, RAISE WARNING IF EITHER ARE NULL!
ropeNode* concatenate(ropeNode* left, ropeNode* right){
	//Create new parent node to attach children to
	ropeNode* newParent = makeRopeNode(NULL);

	//Sets newParent's left and right children
	newParent->left = left;
	newParent->right = right;

	//Return newParent
	return newParent;
};

int characterAt(ropeNode* node, int position){
	if(node->left->weight){
		return 2;
	}
	return 1;
}



int main(){
	return 1;
}
