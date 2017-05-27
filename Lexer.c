#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexi.h"
#include "AST_TREE.h"
#define CHUNK_SIZE 1000
#define SPACES 4

/*
Function of inserting a lexeme. 
*/
int insert_lexeme(lexeme_list** head, lexeme_list** tail, lexeme_type type, char* content, int start, int end)
{
	//The counter is required to analyze the allocated memory("end" subtract "beginning").
	int counter;
	char* q;
	lexeme_list* p;
	//Allocate memory under a lexeme(large).
	p = (lexeme_list*)malloc(sizeof(lexeme_list));
	//If memory can not be allocated, return an error.
	if (NULL == p)
		return 0;
	//Specifies the size of the block of allocated memory.
	counter = end - start;
	//Allocate memory under a lexeme(target).
	q = (char*)malloc(counter + 1);
	//If memory can not be allocated, remove "p" memory + return an error.
	if (NULL == q)
	{
		free(p);
		return 0;
	}
	//Copy from content(starting with the "start") to (q) counter elements
	memmove(q, content + start, counter);
	//Define end of lexeme.
	q[counter] = '\0';
	//Insert type of lexeme.
	p->type = type;
	//Insert value of lexeme.
	p->value = q;
	//Insert pointer to previous lexeme.
	p->previous = *tail;
	//Insert pointer to next lexeme as NULL.
	p->next = NULL;
	//If tail is NULL -> define tail as lexeme(large).
	if (NULL != (*tail))
	{
		(*tail)->next = p;
	}
	//If head is NULL -> define head as lexeme(large).
	if (NULL == (*head))
	{
		*head = p;
	}
	//Define tail as lexeme(large).
	*tail = p;
	return 1;
}

int read_file(char* file_name, char** content, int* file_length)
{
	int allocated = 0;
	char* q = NULL;
	char* p = NULL;
	int numread = 0;
	FILE *f = s_fopen(file_name, "r");
	if (NULL == f)
	{
		return 0;
	}
	*content = NULL;
	*file_length = 0;
	do
	{
		q = realloc(p, allocated + CHUNK_SIZE);
		if (NULL == q)
		{
			fclose(f);
			free(p);
			return 0;
		}
		p = q;
		allocated += CHUNK_SIZE;
		numread = fread(p + *file_length, 1, CHUNK_SIZE, f);
		*file_length += numread;
	} while (numread == CHUNK_SIZE);
	fclose(f);
	*content = (char*)malloc(*file_length);
	if (NULL == *content)
	{
		free(p);
		return 0;
	}
	memmove((*content), p, *file_length);
	free(p);
	return 1;
}

int lexical_analaiser(char* content, lexeme_list** head, lexeme_list** tail)
{
	int start;
	int position = 0;
	int error = 0;
	*head = NULL;
	*tail = NULL;
	while ('\0' != content[position])
	{
		if (isdigit(content[position]))
		{
			start = position;
			while (isdigit(content[position]))
			{
				position++;
			}
			insert_lexeme(head, tail, NUMBER, content, start, position);
		}

		else       if (('+' == content[position]) || ('-' == content[position]))
		{
			start = position;
			position++;
			insert_lexeme(head, tail, OPERATION, content, start, position);
		}
		else
		{
			error = 1;
			break;
		}
	}
	return (!error);
}

int convert_file(char* content, char** content_string, int file_length)
{
	char *p;
	int content_position;
	int p_position;
	p = (char*)malloc(file_length);
	if (NULL == p)
	{
		return 0;
	}

	p_position = 0;
	for (content_position = 0; content_position < file_length; content_position++)
	{
		if (isgraph(content[content_position]))
		{
			p[p_position] = content[content_position];
			p_position++;
		}

	}
	*content_string = (char*)malloc(p_position + 1);
	if (NULL == *content_string)
	{
		free(p);
		return 0;
	}

	memmove(*content_string, p, p_position);
	(*content_string)[p_position] = '\0';
	free(p);
	return 1;
}

void print_tree(FILE* f, AST_tree* root, int count)
{
	if (NULL != root)
	{
		if (root->value != NULL)
		{
			fprintf(f, "%*c<node value=\"%s\" type=\"", count, ' ', root->value);
		}
		else
		{
			fprintf(f, "%*c<node value=\"\" type = \"", count, ' ');
		}
		if (AST_NUMBER == (root->type))

		{
			fprintf(f, "AST_NUMBER\">\n");
		}

		if (AST_OPERATION == (root->type))

		{
			fprintf(f, "AST_OPERATION\">\n");
		}

		if (AST_SUM == (root->type))

		{
			fprintf(f, "AST_SUM\">\n");
		}

		if (AST_REST == (root->type))

		{
			fprintf(f, "AST_REST\">\n");
		}
		print_tree(f, root->left, count + SPACES);
		print_tree(f, root->right, count + SPACES);
		fprintf(f, "%*c</node>\n", count, ' ');
	}
}

int print_tree_to_XML(char* file_name, AST_tree* root)
{
	FILE* f = s_fopen(file_name, "w");
	if (NULL == f)
	{
		perror("fopen");
		return 0;
	}
	fprintf(f, "<?xml version=\"1.0\"?>\n");
	print_tree(f, root, 0);
	fclose(f);
	return 1;
}

int parse_rest_null(lexeme_list** position, int* sintax_error, AST_tree** node)
{
	return 1;
}

int parse_rest(lexeme_list** position, int* sintax_error, AST_tree** node)
{
	if (!create_node(AST_REST, NULL, node))
	{
		*sintax_error = 0;
		return 0;
	}
	if (((*position)->type) != OPERATION)
	{
		*sintax_error = 1;
		return 0;
	}

	if (!create_node(AST_OPERATION, (*position)->value, &((*node)->left)))
	{
		*sintax_error = 0;
		return 0;
	}
	(*position) = (*position)->next;

	return parse_sum(position, sintax_error, &((*node)->right));
}

int parse_sum(lexeme_list** position, int* sintax_error, AST_tree** node)
{
	if (!create_node(AST_SUM, NULL, node))
	{
		*sintax_error = 0;
		return 0;
	}

	if (((*position)->type) != NUMBER)
	{
		*sintax_error = 1;
		return 0;
	}

	if (!create_node(AST_NUMBER, (*position)->value, &((*node)->left)))
	{
		*sintax_error = 0;
		return 0;
	}
	(*position) = (*position)->next;

	if ((*position) == NULL)
	{
		return(parse_rest_null(position, sintax_error, &((*node)->right)));
	}

	if (((*position)->type) == OPERATION)
	{
		return(parse_rest(position, sintax_error, &((*node)->right)));
	}
	else
	{
		(*sintax_error) = 1;
		return 0;
	}
}
