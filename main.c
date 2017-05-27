#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "Lexer.h"
#define CHUNK_SIZE 1000
#define spaces 4

void destroy_list(lexeme_list* head, lexeme_list* tail)
{
	lexeme_list* p;
	lexeme_list* q;

	for (p = head; p != tail;)
	{
		q = p;
		p = p->next;
		free(q->value);
		free(q);
	}
}

void free_ast_tree(AST_tree* root)
{
	if (NULL != root)
	{
		free_ast_tree(root->left);
		free_ast_tree(root->right);
		free(root->value);
		free(root);

	}
}

int main(int argc, char* argv[])
{
	char* content;
	char* content_string;
	int file_length;
	int i;
	int count;
	int sinerror;
	AST_tree* root = NULL;
	lexeme_list* head;
	lexeme_list* tail;
	lexeme_list* copy_head;
	lexeme_list* q;
	head = NULL;
	tail = NULL;
	if (3 == argc)
	{
		if (read_file(argv[1], &content, &file_length))
		{
			convert_file(content, &content_string, file_length);
			if (lexical_analaiser(content_string, &head, &tail))
			{
				copy_head = head;
				if (parse_sum(&copy_head, &sinerror, &root))
				{

					if (!(print_tree_to_XML(argv[2], root)))
					{
						printf("file not write");
					}

				}
				else
				{
					if (sinerror)
					{
						printf("sintaxis error");
					}
					else
					{
						printf("programm error");
					}
				}
				free_ast_tree(root);

			}
			else
			{
				printf("error lexical_analaxer");
			}
			destroy_list(head, tail);
			free(content_string);
			free(content);
		}

		else
		{
			printf("Error!");
		}
	}
	else
	{
		printf("Wrong number of arguments");
	}
}

