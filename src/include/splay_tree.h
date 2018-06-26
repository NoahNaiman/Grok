#ifndef SPLAY_TREE_H
#define SPLAY_TREE_H

typedef struct SplayTree_t SplayTree_t;

struct SplayTree_t{
	int buffer;
	int physicalStart;
	int logicalStart;
	int length;
	SplayTree_t *left;
	SplayTree_t *right;
};
SplayTree_t* init_splay_tree(int whichBuffer, int physicalStart, int logicalStart, int length);

SplayTree_t* insert(SplayTree_t* root, SplayTree_t* newNode);

SplayTree_t* destroy(SplayTree_t* root, int key);

SplayTree_t* left_rotate(SplayTree_t* node);

SplayTree_t* right_rotate(SplayTree_t* node);

SplayTree_t* splay(SplayTree_t* root, int index);

void traverse_preorder(SplayTree_t* root);

void traverse_inorder(SplayTree_t* root);

void traverse_postorder(SplayTree_t* root);

#endif