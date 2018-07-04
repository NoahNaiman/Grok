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

/*
 * init_splay_tree Usage
 * --------------------------------
 * Initialize a lone SplayTree_t node by
 * recording details for a span of text
 * relevant for usage in a PieceChain_t
 *
 * Example:
 *	SplayTree_t* aNewSplayTree = init_splay_tree(whichBuffer, physicalStart, logicalStart, length);
 *	-whichBuffer = an integer 0 if span of text is in original buffer, 1 if in add buffer
 *	-physicalStart = an integer representing where span of texts starts in corresponding buffer
 *	-logicalStart = an integer representing where span logically starts in text
 *	_length = an integer representing the number of characters in given span
 */
SplayTree_t* init_splay_tree(int whichBuffer, int physicalStart, int logicalStart, int length);

/*
 * insert Usage
 * --------------------------------
 * Inserts a SplayTree_t node into an existing
 * SplayTree_t, splaying the newly inserted node
 *
 * Example:
 *	anExistingSplayTree_t = insert(anExistingSplayTree_t, aNewNode);
 *	-anExistingSplayTree_t = an already initialized SplayTree_t
 *	-aNewNode = a new SplayTree_t node not yet part of previous tree
 */
SplayTree_t* insert(SplayTree_t* root, SplayTree_t* newNode);

/*
 * remove Usage
 * --------------------------------
 * Splays a SplayTree_t node to root in an existing
 * SplayTree_t, then frees the given node from memory
 * and recombines each half of the tree
 *
 * Example:
 *	anExistingSplayTree_t = remove(anExistingSplayTree_t, key);
 *	-anExistingSplayTree_t = an already initialized SplayTree_t
 *	-key = an integer representing the logical start for a span
 *	 of text whose corresponding node should be removed
 */
SplayTree_t* destroy(SplayTree_t* root, int key);

#endif