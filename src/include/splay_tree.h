#ifndef SPLAY_TREE_H
#define SPLAY_TREE_H

/* 
 * Splay Tree Definition
 * ----------------------
 * Summary:
 *	A special case of a binary search tree,
 *	optimized for localized oprations. Whenever
 *	an operation is performed on a node, that
 *	node is first "splayed" to the top of the
 *	tree by repeatedly roating it, its parent
 *	node, and its children right and left until
 *	it is the new root. Each node in this tree
 *	holds information regarding a span of text
 *	to be used for a piece chain data structure.
 *
 * Typdef:
 *	SplayTree_t
 *
 * Values:
 *	int buffer;
 *		-An integer representing in which buffer
 *		 this node's corresponding text is stored
 *			--0 if original
 *			--1 if add
 *	int physicalStart:
 *		-An integer representing the start index of
 *		 this node's span of text in its corresponding
 *		 buffer
 *	int logicalStart:
 *		-An integer representing the start index of
 *		 this node's span of text within the context
 *		 of the text represented by the entire tree
 *	int length:
 *		-An integer representing the number of characters
 *		 in this node's span of text
 *
 *
 * Visual Example:
 *	
 *	Original file buffer (read only):
 *
 *	-----------------------------------------
 *	|A| |l|a|r|g|e| |s|p|a|n| |o|f| |t|e|x|t|
 *	-----------------------------------------
 *
 *	Add buffer (append only):
 *
 *	-----------------------------------------
 *	|E|n|g|l|i|s|h|							|
 *	-----------------------------------------
 *
 *	Tree
 *
 *							  --------------------
 *						      | buffer: 1	     |
 *							  | physicalStart: 0 | 
 *							  | logicalStart: 11 |
 *							  | length: 8		 |
 *							  --------------------
 *					              /			 \
 *					             /	          \ 
 *					            / 			   \
 *			--------------------				--------------------
 *		 	| buffer: 0 	   |				| buffer: 0 	   |
 *		 	| physicalStart: 8 |				| physicalStart: 16| 
 * 		 	| logicalStart: 3  |				| logicalStart: 18 |
 *		 	| length: 8		   |				| length: 4		   |
 *		 	--------------------				--------------------
 *		  	  	    /
 *		 	 	   /
 *			      /
 *	--------------------
 *	| buffer: 0 	   |
 *	| physicalStart: 0 | 
 *	| logicalStart: 0  |
 *	| length: 2		   |
 *	--------------------
 *
 * 	Text:
 *
 * 	A span of English text
 */
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