#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/splay_tree.h"

/* INITIALIZER */

SplayTree_t* init_splay_tree(int whichBuffer, int physicalStart, int logicalStart, int length){
	SplayTree_t* newTree = (SplayTree_t *)malloc(sizeof(SplayTree_t));

	newTree->buffer = whichBuffer;
	newTree->physicalStart = physicalStart;
	newTree->logicalStart = logicalStart;
	newTree->length = length;
	newTree->left = NULL;
	newTree->right = NULL;

	return newTree;
}

/* UTILITY FUNCTIONS */

/*
 * left_rotate Function Definition
 * --------------------------------
 * Function Summary:
 *	Shifts a given SplayTree_t node upwards and left,
 *	adjusting tree as necessary
 *
 *	     b                              d
 *   	/ \      Left Rotate 'd'       / \
 *     a   d   ------------------->   b   e 
 *        / \                 		 / \
 *       c   e              	    a	c
 *
 * Parameters:
 *	SplayTree_t* node
 *		-A pointer to a SplayTree_t
 *		-May not be NULL
 */
SplayTree_t* left_rotate(SplayTree_t* node){
	SplayTree_t* temp = node->right;
	node->right = temp->left;
	temp->left = node;
	return temp;
}


/*
 * right_rotate Function Definition
 * --------------------------------
 * Function Summary:
 *	Shifts a given SplayTree_t node upwards and right,
 *	adjusting tree as necessary
 *
 *	     d                              b
 *   	/ \      Right Rotation        / \
 *     b   e   ------------------->   a   d 
 *    / \                 		         / \
 *   a   c              	            c	e
 *
 * Parameters:
 *	SplayTree_t* node
 *		-A pointer to a SplayTree_t
 *		-May not be NULL
 */
SplayTree_t* right_rotate(SplayTree_t* node){
	SplayTree_t* temp = node->left;
	node->left = temp->right;
	temp->right = node;
	return temp;
}

/*
 * consolidate Function Definition
 * --------------------------------
 * Function Summary:
 *	Merges a node with one or both of its
 *	children if they directly follow one
 *	another both in memory and logically
 *
 * Parameters:
 *	SplayTree_t* root
 *		-A pointer to a SplayTree_t
 *		-May not be NULL
 */
SplayTree_t* consolidate(SplayTree_t* root){
	int rootEndPhysical = root->physicalStart + root->length;
	int rootEndLogical = root->logicalStart + root->length;
	int leftEndPhysical = root->left->physicalStart + root->left->length;
	int leftEndLogical = root->left->logicalStart + root->left->length;
	
	if((leftEndPhysical + 1) == root->physicalStart &&
		(leftEndLogical + 1) == root->logicalStart &&
		(rootEndPhysical + 1) == root->right->physicalStart &&
		(rootEndLogical + 1) == root->right->logicalStart &&
		root->left->buffer == root->buffer &&
		root->buffer == root->right->buffer){
		//consolidate left, right, center
		root->physicalStart = root->left->physicalStart;
		root->logicalStart = root->left->logicalStart;
		root->length = root->left->length + root->length + root->right->length;

		root->left = destroy(root->left, root->left->logicalStart);
		root->right = destroy(root->right, root->right->logicalStart);
	}
	else if((leftEndPhysical + 1) == root->physicalStart &&
		(leftEndLogical + 1) == root->logicalStart &&
		root->left->buffer == root->buffer){

		root->physicalStart = root->left->physicalStart;
		root->logicalStart = root->left->logicalStart;
		root->length = root->left->length + root->length;

		root->left = destroy(root->left, root->left->logicalStart);
	}
	else if((rootEndPhysical + 1) == root->right->physicalStart &&
		(rootEndLogical + 1) == root->right->logicalStart &&
		root->buffer == root->right->buffer){

		root->length = root->length + root->right->length;

		root->right = destroy(root->right, root->right->logicalStart);
	}
	return root;
}

/*
 * splay Function Definition
 * --------------------------------
 * Function Summary:
 *	Takes a SplayTree_t node who's logicalStart is closest
 *	to given key and repeatedly shifts tree until it is
 *	the root node
 *
 *	     	 f                              a
 *   		/ \            Splay        	 \
 *     	   d   e   ------------------->   	  d
 *    	  /                  		         / \
 *   	 c 									b 	f
 *	   	/									 \	 \
 *	   b 									  c   e
 *	  /
 *   a             	            
 *
 * Parameters:
 *	SplayTree_t* node
 *		-A pointer to a SplayTree_t
 *		-May not be NULL
 *	int key
 *		-An integer representing a logical start
 *		 index whose corresponding node should be
 *		 splayed to the top of the tree
 */
SplayTree_t* splay(SplayTree_t* node, int key){
	if(node == NULL || node->logicalStart == key){
		return node;
	}

	if(node->logicalStart > key){
		if(node->left == NULL){
			return node;
		}

		if(node->left->logicalStart > key){
			node->left->left = splay(node->left->left, key);
			node = right_rotate(node);
		}
		else if(node->left->logicalStart < key){
			node->left->right = splay(node->left->right, key);

			if(node->left->right != NULL){
				node->left = left_rotate(node->left);
			}
		}
		return node->left == NULL ? node: right_rotate(node);
	}
	else{
		if(node->right == NULL){
			return node;
		}

		if(node->right->logicalStart > key){
			node->right->left = splay(node->right->left, key);

			if(node->right->left != NULL){
				node->right = right_rotate(node->right);
			}
		}
		else if(node->right->logicalStart < key){
			node->right->right = splay(node->right->right, key);
			node = left_rotate(node);
		}
		return node->right == NULL ? node: left_rotate(node);
	}
}

/*
 * increment Function Definition
 * --------------------------------
 * Function Summary:
 *	Increments by shift the logical start of
 *	given node and all children in node's right
 *	subtree
 *
 * Parameters:
 *	SplayTree_t* node
 *		-A pointer to a SplayTree_t
 *		-May not be NULL
 *	int shift
 *		-An integer by which to increment the
 *		 logical start of node and node's
 *		 right subtree by
 */
void increment(SplayTree_t* node, int shift){
	if(node == NULL){
		return;
	}
	node->logicalStart = node->logicalStart + shift;
	increment(node->right, shift);
	increment(node->left, shift);
	return;
}

/* CORE FUNCTIONS */

/*
 * insert Function Definition
 * --------------------------------
 * Function Summary:
 *	Inserts a new SplayTree_t node into a given
 *	tree, pre-splaying the tree with the node
 *	whose logicalStart is closest to new node's
 *
 *	     	 e                              b
 *   		/ \          Insert 'b'        / \
 *     	   f   g   ------------------->   a	  f
 *    	  /                  		         / \
 *   	 d 									c 	e
 *	   	/									 \	 \
 *	   c 									  d   g
 *	  /
 *   a             	            
 *
 * Parameters:
 *	SplayTree_t* root
 *		-A pointer to an existing SplayTree_t
 *		-May not be NULL
 *	SplayTree_t* newNode
 *		-A newly created pointer a SplayTree_t
 *		 to be inserted
 *		-May not be NULL
 */
SplayTree_t* insert(SplayTree_t* root, SplayTree_t* newNode){
	if(root == NULL){
		return newNode;
	}

	int key = newNode->logicalStart;
	int rootLogical = root->logicalStart;
	int newNodeLength = newNode->length;

	root = splay(root, key);

	if(rootLogical == key){
		root->logicalStart = rootLogical + newNodeLength; 
		increment(root->right, newNodeLength); 
		newNode->right = root;
		newNode->left = root->left;
		root->left = NULL;
		return newNode;
	}
	else if(rootLogical > key){
		//Repeat splay to get node with closest but less logical start
		root = splay(root, key);
	}
	
	int preFractureLength = newNode->logicalStart - root->logicalStart;
	int pyhsicalFracture = root->physicalStart + preFractureLength + newNodeLength;
	int logicalFracture = root->logicalStart + preFractureLength + newNodeLength; 
	int postFractureLength = root->length - preFractureLength;

	SplayTree_t* postFractureNode = init_splay_tree(root->buffer, pyhsicalFracture, logicalFracture, postFractureLength);
	increment(root->right, newNodeLength); 
	postFractureNode->right = root->right;

	root->length = preFractureLength;
	root->right = NULL;

	newNode->right = postFractureNode;
	newNode->left = root;
	newNode = consolidate(newNode);
	return newNode;
}

/*
 * destroy Function Definition
 * --------------------------------
 * Function Summary:
 *	Splays a node with the given key, then
 *	removes it from tree and rejoins existing
 *	pieces
 *
 *	     	 d                              b
 *   		/ \          Delete 'c'        / \
 *     	   a   e   ------------------->   a	  d
 *    	    \                 		           \
 *   	  	 c								 	e
 *	   		/								   
 *	       b								  
 *	  
 * Parameters:
 *	SplayTree_t* root
 *		-A pointer to a SplayTree_t
 *		-May not be NULL
 *	int key
 *		-An integer representing a starting location
 *		 whose corresponding span should be deleted
 */
SplayTree_t* destroy(SplayTree_t* root, int key){
	SplayTree_t* temp;

	if(root == NULL){
		return NULL;
	}

	root = splay(root, key);

	//Key is not in tree
	if(key != root->logicalStart){
		return root;
	}

	if(root->left == NULL){
		temp = root;
		root = root->right;
	}
	else{
		temp = root;
		root = splay(root->left, key);
		root->right = temp->right;
	}

	free(temp);

	return root;
}
