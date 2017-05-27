#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "Lexer.h"



int create_node(node_type t, char* v, AST_tree** node)
{
	//We allocate memory under the node.
	(*node) = (AST_tree*)malloc(sizeof(AST_tree));
	//If the node is NULL -> return an error
	if (NULL == (*node))
		return 0;
	//Define the type of the lexem.
	((*node)->type) = t;
	//Define node leaf as NULL
	(*node)->right = NULL;
	(*node)->left = NULL;
	//If value is NULL -> define the value of the node as a copy(pointer).
	if (v != NULL)
	{
		(*node)->value = strdup(v);
		//if value is NULL -> destroy node(free memory) and return an error.
		if (NULL == (*node)->value)
		{
			free(*node);
			(*node) = NULL;
			return 0;
		}
	}
	//If value is NULL -> define the value of the node as a NULL.
	else
		(*node)->value = NULL;
	return 1;
}





