#include "naive.h" 

int  main ( int argc, char **argv ) {

	char* str;

	if ( argc!=2 
	   || (str = malloc(strlen(argv[1])*sizeof(char))) == NULL 
	   || sscanf(argv[1], "%s", str)!=1 ){ 

        fprintf(stderr,"./suffixtree string\n");
        return -1;

    }

	printf( "%s", cJSON_Print( treeToJSON( getSuffixTree(str))));

    return 0;
}