#include<string.h>
#include<stdio.h>
#include<stdlib.h>
//TODO: CUT FUNCTION

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

//TODO: SET WEIGHT, RAISE WARNING IF EITHER ARE NULL, ENSURE ALL CHILDREN ARE FULL
ropeNode* concatenate(ropeNode* left, ropeNode* right){
	//Create new parent node to attach children to
	ropeNode* newParent = makeRopeNode(NULL);

	//Sets newParent's left and right children
	newParent->left = left;
	newParent->right = right;

	//Return newParent
	return newParent;
};

/*
 * characterAt Function Definition
 * --------------------------------
 * Function Summary:
 *	Searches for the character in a string at a given index
 *
 * Parameters:
 *	ropeNode* root
 *		-A pointer to a ropeNode whose string represents the
 *		 text to be searched
 *		-May be NULL
 *	int position
 *		-The index whose corresponding character should be returned
 *
 * Return Type:
 *	char
 *		-A character at the specified position
 */
char characterAt(ropeNode* root, int position){
	//If position is out of bounds return NULL character
	if(!root && (position > root->weight) && (root->right == NULL)){
		return '\0';
	}

	//Decide which direction to recurse down, update position as necessary
	if(root->left->weight <= position){
		return characterAt(root->right, (position - root->weight));
	}
	else if(root->left != NULL){
		return characterAt(root->left, position);
	}

	//Return char at given position
	return root->string[position];
}

/*
 * stringLength Function Definition
 * --------------------------------
 * Function Summary:
 *	-Gets the length of the string held in the leaves of a given ropeNode
 *	-Should only be called on the left child of a ropeNode to start scan
 *
 * Parameters:
 *	ropeNode* root
 *		-A pointer to a ropeNode whose string represents the
 *		 text to scanned
 *		-May be NULL
 *
 * Return Type:
 *	int
 *		-The length of a given ropeNode's string length
 */
int stringLength(ropeNode* root){
	//Check if given root node is NULL
	if(root == NULL){
		return 0;
	}
	//If root has children, recurse down it
	if(root->string == NULL){
		return stringLength(root->left) + stringLength(root->right);
	}
	//If root has a string, return its length
	return root->weight;
}

/*
 * printString Function Definition
 * --------------------------------
 * Function Summary:
 *	Prints string starting from node to standard output
 *
 * Parameters:
 *	ropeNode* root
 *		-A pointer to a ropeNode whose string should be printed, or to be
 *	     recursed downwards from
 */
//TODO: COME BACK TO THIS ONCE FULL FILE IS CREATED, STORE OUTPUT? GET STRING?
void printString(ropeNode* root){
	//Stop if given node is NULL
	if(root == NULL){
		return;
	}
	//Check if node has a string to print, only leaves will
	if(root->string != NULL){
		printf("%s", root->string);
	}
	//Recurse down tree
	printString(root->left);
	printString(root->right);
}



int main(){
	return 1;
}
