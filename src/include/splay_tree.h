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

SplayTree_t* right_rotate(SplayTree_t* node);

#endif