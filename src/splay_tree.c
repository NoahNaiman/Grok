#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#include "include/splay_tree.h"

/* Core Functions */

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

	root = splay(root, key);

	if(root->logicalStart == key){
		return root;
	}

	if(root->logicalStart > key){
		newNode->right = root;
		newNode->left = root->left;
		root->left = NULL;
	}
	else{
		newNode->left = root;
		newNode->right = root->right;
		root->right = NULL;
	}
	return newNode;
}

/*
 * delete Function Definition
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
SplayTree_t* delete(SplayTree_t* root, int key){
	SplayTree_t* temp;

	if(root == NULL){
		return NULL;
	}

	root = splay(root, key);

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

/* Utility Functions */

/*
 * left_rotate Function Definition
 * --------------------------------
 * Function Summary:
 *	Shifts a given SplayTree_t node upwards and left,
 *	adjusting tree as necessary
 *
 *	     b                              d
 *   	/ \      Left Rotation         / \
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

void traverse_inorder(SplayTree_t* root){
	if(root == NULL){
		return;
	}
	traverse_inorder(root->left);
	printf("%d ", root->logicalStart);
	traverse_inorder(root->right);
}