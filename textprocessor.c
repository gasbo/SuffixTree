#include <stdio.h>
#include "naive.h"

int counter(struct NODE* node){

	int count = 0;
	for (int i = 0; i < ALPHABET_WIDTH; ++i)
	{
		if (node->edges[i] != NULL ){
			count += node->edges[i]->rgt - node->edges[i]->lft + (node->edges[i]->rgt == _WIDTH ? 0 : 1);
			count += counter(node->edges[i]->to);
		}
	}
	return count;

}

int main (){

	char text[100000] = {0};
	char p_text[10000] = {0};
	int Q,W, start, i;

	int result;
	struct NODE* tree;

	scanf("%s", text);
	scanf("%d %d", &Q, &W);
	// TESTING
	//srand(time(NULL));
	for ( i = 0; i < Q; ++i)
	{
		scanf("%d", &start);

		strcpy(p_text, text + start);
		p_text[W] = 0;

		tree = getSuffixTree(p_text);

		result = counter(tree);

		freeTree(tree);

		printf("%d\n", result);
		
	}

	return 0;
}