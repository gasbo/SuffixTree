#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h" // https://github.com/DaveGamble/cJSON

#define FIRST_LETTER 'a'
#define LAST_LETTER 'z'
#define TERMINATOR '$'
#define ALPHABET_WIDTH (LAST_LETTER - FIRST_LETTER)


char* _TEXT;
struct NODE* _ROOT;
long _WIDTH;


typedef struct EDGE
{
    int lft;
    int rgt;    
    struct NODE* from;
    struct NODE* to;


} EDGE;

typedef struct NODE
{
	struct EDGE* edges[ALPHABET_WIDTH + 1];

} NODE;


struct EDGE* getEdge(struct NODE* node, char c ){

	if (c == TERMINATOR){

 		return node->edges[ALPHABET_WIDTH];

 	} else{

 		return node->edges[ c - FIRST_LETTER];
 	}

}

void setEdge(struct NODE* node, char c, struct EDGE* edge ){

	int i;
	
	if (c == TERMINATOR){

		i = ALPHABET_WIDTH;

 	} else{

 		i = c - FIRST_LETTER;
 	}

 	node->edges[i] = edge;
 	edge = getEdge(node, c);

 	if( edge == NULL ){
 		free(edge);
 	}

}

struct NODE* allocNode(){

 	struct NODE* son = malloc(sizeof(struct NODE));

 	for (int i = 0; i < ALPHABET_WIDTH + 1; ++i)
 	{
 		son->edges[i] = NULL;
 	}

 	return son;
}

void addEdge(struct NODE* from, struct NODE* to, int lft, int rgt){

	struct EDGE* edge  = malloc(sizeof(struct EDGE));

 	edge->lft = lft;
 	edge->rgt = rgt;
 	edge->from = from;
 	edge->to = to;

	setEdge(from, _TEXT[lft], edge);
}

void addSon(struct NODE* parent, int lft, int rgt){

 	addEdge(parent, allocNode(), lft, rgt);
}

struct NODE* splitEdge(struct EDGE* to_cut, int shared_chars){

 	struct NODE* center = allocNode();

 	addEdge(to_cut->from, center, to_cut->lft, to_cut->lft + shared_chars - 1);
 	addEdge(center, to_cut->to, to_cut->lft + shared_chars, to_cut->rgt);

 	free(to_cut);

 	return center;
}

/*
 * Function:  searchPrefix 
 * -----------------------------------
 * 	Given a suffix, looks for the edge with the most similar suffix.
 *
 *  depth: the index of the first char of the suffix.
 * 
 *  node: the node.
 *  
 *  last_edge: the last edge which shared chars with the suffix.
 *
 *  last_shared: number of chars shared in the last consulted edge.
 *
 *  returns: Number of chars of the suffix already present in the tree.
 */
int searchPrefix(int depth, struct NODE* node, struct EDGE** last_edge, int* last_shared){

	struct EDGE* edge = getEdge(node, _TEXT[depth]);
	int i, w;


	if ( edge != NULL ){


		w = edge->rgt - edge->lft + 1;

		for (i = 0; i < w; ++i)
		{
			if (_TEXT[depth + i] != _TEXT[edge->lft + i]) {
				*last_shared = i;
				*last_edge = edge;
				return i; //number of shared chars
			}
		}
		*last_edge = edge;
		*last_shared = i;

		return i + searchPrefix(depth+w, edge->to, last_edge, last_shared);

	}
	*last_shared = 0;

	return 0;

}

/*
 * Function:  getSuffixTree 
 * -----------------------------------
 * 	Computes the suffix tree using the naive algorithm.
 *
 *  str: the plain string, without the termination char in the end.
 *
 *  returns: the suffix tree: It can be explored with the method treeCrawler().
 * 			 It's also possible to print it in JSON format with treeToJSON().
 */
struct NODE* getSuffixTree( char* str ){

	_TEXT = str;
	_WIDTH = strlen(_TEXT);
	_TEXT[_WIDTH] = '$';
	_ROOT = allocNode();

	struct EDGE* last_edge = NULL;
	struct NODE* node;
	int total_shared_chars, last_shared_chars;

	for (int i = 0; i <= _WIDTH; ++i)
	 {
	 	last_edge = NULL;

	 	total_shared_chars = searchPrefix(i, _ROOT, &last_edge, &last_shared_chars);

	 	if (last_edge == NULL){

			addSon(_ROOT, i, _WIDTH);

	 	} else {

		 	if (last_shared_chars == 0)
		 	{
		 		addSon(last_edge->to, i + total_shared_chars, _WIDTH); 

		 	} else {

		 		node = splitEdge(last_edge, last_shared_chars);
		 		addSon(node, i + total_shared_chars, _WIDTH);

		 	}
		 }

	 }

	 return _ROOT;
}

/*
 * Function:  treeCrawler 
 * -----------------------------------
 * 	Crawls over the tree following the inputs from the user. For testing purposes.
 */
void treeCrawler(struct NODE* node){
	struct EDGE* out;
	int l;

	printf("NODE %p\n",node);

	for (int i = 0; i < ALPHABET_WIDTH + 1; ++i)
	{
		if(node->edges[i] != NULL){

			out = node->edges[i];
			printf("%d: edge %p-%.*s (%d -- %d)\n", i, out, 1 + out->rgt - out->lft, _TEXT + out->lft, out->lft, out->rgt);
			
		}
	}

	scanf("%d", &l);

	if ( l >= 0 && l <= ALPHABET_WIDTH && node->edges[l] != NULL){

		treeCrawler(node->edges[l]->to);

	} else {
				return;//treeCrawler(_ROOT);

	}
	
}


/*
 * Function:  treeToJSON 
 * -----------------------------------
 * 	Generates a cJSON object which can be printed and compared in order to do some automatic testing with another algorithms.
 */
cJSON* treeToJSON(struct NODE* node){

	cJSON *edges = cJSON_CreateArray();
	cJSON *origin = cJSON_CreateObject();
	cJSON *json_node = NULL;
	char* label;
	int w;

	for (int i = 0; i < ALPHABET_WIDTH + 1; ++i)
	{
		if(node->edges[i] != NULL){

			json_node = cJSON_CreateObject();

			w = node->edges[i]->rgt - node->edges[i]->lft + 1;
			label = malloc((w+1)*sizeof(char));
			strncpy(label, _TEXT + node->edges[i]->lft, w);
			label[w] = '\0';

			cJSON_AddItemToObject(json_node, "label", cJSON_CreateString(label));
			cJSON_AddItemToObject(json_node, "to", treeToJSON(node->edges[i]->to));

			cJSON_AddItemToArray( edges, json_node);			
		}
	}

	cJSON_AddItemToObject(origin, "edges", edges);


	return origin;
	
}