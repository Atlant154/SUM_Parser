#pragma once

typedef enum t
{ 
	NUMBER,
	OPERATION
}lexeme_type; 

/*
Create a list structure. 
Define the type of the token, its value, and give pointers to the previous and next element of the list. 
We get a mutually connected list.
*/
typedef struct list
{
	lexeme_type type;
	char* value;
	struct list* previous;
	struct list* next;
}lexeme_list;

/*
Define the type of the tree node. 
In this case, there is a division into the current tree and the "rest". 
Dynamic (balanced) filling of the tree is not used.
*/
typedef enum type
{
	AST_NUMBER,
	AST_OPERATION,
	AST_SUM,
	AST_REST,
} node_type;

/*
Define the structure of the tree. 
The node type, value, and pointers to the left and right branch.
*/
typedef struct tree
{
	node_type type;
	char* value;
	struct tree* left;
	struct tree* right;
}AST_tree;


int insert_lexeme(lexeme_list** head, lexeme_list** tail, lexeme_type type, char* content, int start, int end);
int read_file(char* file_name, char** content, int* file_length);
int lexical_analaiser(char* content, lexeme_list** head, lexeme_list** tail);
int convert_file(char* content, char** content_string, int file_length);
void print_tree(FILE* f, AST_tree* root, int count);
int print_tree_to_XML(char* file_name, AST_tree* root);
//(!)
int parse_rest_null(lexeme_list** position, int* sintax_error, AST_tree** node);
int parse_rest(lexeme_list** position, int* sintax_error, AST_tree** node);
int parse_sum(lexeme_list** position, int* sintax_error, AST_tree** node);
