#include <stdio.h>
#include "naive.h"

long long counter(struct NODE* node){

	long long count = 0;
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
	char c;
	int Q,W, start, i;

	struct NODE* tree;

	scanf("%s", text);
	scanf("%d %d", &Q, &W);
	// TESTING
	//srand(time(NULL));
	for ( i = 0; i < Q; ++i)
	{
		scanf("%d", &start);

		c = *(text + start + W);
		*(text + start + W) = 0;

		tree = getSuffixTree(text + start);

		*(text + start + W) = c;

		printf("%lld\n", counter(tree));

		freeTree(tree);

		// printf("%.*s\n", W, text + start);
		
	}

	return 0;
}