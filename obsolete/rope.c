#include<string.h>
#include<stdio.h>
#include<stdlib.h>

/*
 * Rope Definition
 * ---------------
 * Summary:
 *	A binary tree wherein each leaf contains a string
 *	representing one word. The weight of each node is
 *	the length of its word if a leaf, and the total weight
 *	of all leaves in its left subtree if not.
 *
 * Typdef:
 *	ropeNode
 *
 * Values:
 *	char word[]
 *		-Leaf: Characters representing a single word of text
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
typedef struct Rope {
	char *word;
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
ropeNode* makeRopeNode(char* string){

	//Allocate space for new ropeNode
	ropeNode* newNode = (ropeNode*)malloc(sizeof(ropeNode));
	
	//Assign given string to newNode
	newNode->word = string;

	//Assign weight to newNode, zero if words is NULL
	newNode->weight = !string ? strlen(string) : 0;

	//Set newNode's children as NULL
	newNode->left = NULL;
	newNode->right = NULL;

	//Return newly constructed
	return(newNode);
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
 *		-The length of a given ropeNode's word
 */
int stringLength(ropeNode* root){
	//Check if given root node is NULL
	if(root == NULL){
		return 0;
	}
	//If root has children, recurse down it
	if(root->word == NULL){
		return stringLength(root->left) + stringLength(root->right);
	}
	//If root has a word, return its length
	return root->weight;
}

/*
 * stringConcatenate Function Definition
 * --------------------------------
 * Function Summary:
 *	-A utility function to concatenate two strings into dest
 *
 * Parameters:
 *	char* dest
 *		-A pointer to an array of chars to write
 *		-May not be NULL
 *	char* src
 *		-A pointer to an array of chars to write to dest
 *		-May not be NULL
 *
 * Return Type:
 *	char*
 *		-A pointer to the newly concatenated string
 */
char* stringConcatenate(char* dest, char* src){
	while(*dest){
		dest++;
	}
	while((*dest++ = *src++));
	return --dest;
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
 *		-Sets newParent's *word to NULL
 *		-Sets newParent's weight to the length of the string
 *		 in its left subtree
 *
 * Parameters:
 *	ropeNode* left
 *		-A pointer to a ropeNode whose string represents the
 *		 former part of some text
 *		-Should never be NULL as this could cause unintended
 *		 side effects
 *	ropeNode* right
 *		-A pointer to a ropeNode whose string represents the
 *		 latter part of some text
 *		-Should never be NULL as this could cause unintended
 *		 side effects
 *
 * Return Type:
 *	ropeNode*
 *		-A newly created rope node whose children represent
 *		 two parts of a newly concatenated string
 */
ropeNode* concatenate(ropeNode* left, ropeNode* right){
	//If left node has an empty space, add right node there
	if(left->word == NULL){
		if(left->right == NULL){
			left->right = right;
			return left;
		}
	}

	//Create new parent node to attach children to
	ropeNode* newParent = makeRopeNode(NULL);

	//Sets newParent's right and left nodes
	newParent->left = left;
	newParent->right = right;

	//Sets newParent's weight by getting string length of left child
	newParent->weight = stringLength(newParent->left);

	//Return newParent
	return newParent;
};

/*
 * isBalanced Function Definition
 * --------------------------------
 * Function Summary:
 *	Determines balance of rope defined as the absolute difference in height
 *	between left and right sub-trees being no greater than one
 *		-A rope is balanced if every ropeNode has either two or no children
 *
 * Parameters:
 *	ropeNode* root
 *		-A pointer to a ropeNode representing where to start checking
 *	     for balance
 *		-May be NULL
 *
 * Return Type:
 *	int
 *		-Height of tree if balanced
 *		-(-1) if unbalanced
 */
int isBalanced(ropeNode* root){
	//Check if root is NULL
	if(root == NULL){
		return 0;
	}

	//Check that left subtree is balanced
	int leftSubtreeHeight = isBalanced(root->left);
	if(leftSubtreeHeight == -1){
		return -1;
	}

	//Check that right subtree is balanced
	int rightSubtreeHeight = isBalanced(root->right);
	if(rightSubtreeHeight == -1){
		return -1;
	}

	//Check if full subtree of root is balanced
	if(-1*(leftSubtreeHeight - rightSubtreeHeight) > 0){
		return -1;
	}

	//Get larger height between two subtrees and incremen
	int balanceDifference = leftSubtreeHeight > rightSubtreeHeight ? leftSubtreeHeight+1 : rightSubtreeHeight+1;
	return balanceDifference+1;
}

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
	return root->word[position];
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
	if(root->word != NULL){
		printf("%s", root->word);
	}
	//Recurse down tree
	printString(root->left);
	printString(root->right);
}



int main(){
	return 1;
}
