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

/* Utility Functions */

SplayTree_t* left_rotate(SplayTree_t* node){
	SplayTree_t* temp = node->right;
	node->right = temp->left;
	temp->left = node;
	return temp;
}

SplayTree_t* right_rotate(SplayTree_t* node){
	SplayTree_t* temp = node->left;
	node->left = temp->right;
	temp->right = node;
	return temp;
}

//TODO: ROPE STYLE INDEX DECRMENT?
SplayTree_t* splay(SplayTree_t* root, int index){
	if(root == NULL || root->logicalStart == index){
		return root;
	}

	if(root->logicalStart > index){
		if(root->left == NULL){
			return root;
		}

		if(root->left->logicalStart > index){ //Zig-Zig (Left-Left)
			root->left->left = splay(root->left->left, index);
			root = right_rotate(root);
		}
		else if(root->left->logicalStart < index){ //Zig-Zag (Left-Right)
			root->left->right = splay(root->left->right, index);

			if(root->left->right != NULL){
				root->left = left_rotate(root->left);
			}
		}
		return root->left == NULL ? root: right_rotate(root);
	}
	else{
		if(root->right == NULL){
			return root;
		}

		if(root->right->logicalStart > index){
			root->right->left = splay(root->right->left, index);

			if(root->right->left != NULL){
				root->right = right_rotate(root->right);
			}
		}
		else if(root->right->logicalStart < index){
			root->right->right = splay(root->right->right, index);
			root = left_rotate(root);
		}
		return root->right == NULL ? root: left_rotate(root);
	}
}


