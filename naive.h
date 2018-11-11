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


struct EDGE
{
    int lft;
    int rgt;    
    struct NODE* from;
    struct NODE* to;


};

struct NODE {

	struct EDGE* edges[ALPHABET_WIDTH + 1];

};


struct EDGE* getEdge(struct NODE* node, char c );

void setEdge(struct NODE* node, char c, struct EDGE* edge );

struct NODE* allocNode();

void addEdge(struct NODE* from, struct NODE* to, int lft, int rgt);

void addSon(struct NODE* parent, int lft, int rgt);

struct NODE* splitEdge(struct EDGE* to_cut, int shared_chars);

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
int searchPrefix(int depth, struct NODE* node, struct EDGE** last_edge, int* last_shared);

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
struct NODE* getSuffixTree( char* str );

/*
 * Function:  treeCrawler 
 * -----------------------------------
 * 	Crawls over the tree following the inputs from the user. For testing purposes.
 */
void treeCrawler(struct NODE* node);

/*
 * Function:  treeToJSON 
 * -----------------------------------
 * 	Generates a cJSON object which can be printed and compared in order to do some automatic testing with another algorithms.
 */
cJSON* treeToJSON(struct NODE* node);