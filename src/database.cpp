#include "database.h"
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int FileSize(FILE *file)
{
    assert(file != NULL);

	int _file_size = -1;

	fseek(file, 0, SEEK_END);
	_file_size = ftell(file);

	return _file_size;
}

// mode = 1, if ans, mode = 0, if question
Node* ReadMakeNode(char mode, char* begin, char* end, Tree *tree)
{
    assert(begin != nullptr);
    assert(end != nullptr);
    assert(end - begin > 0);
    
    *end = 0;
    struct Node *node = MakeNode(mode, AddNewLine(begin + 1, tree));
    *end = '\"';
    
    return node;
}

char* GetNextChar(char *line)
{
    assert(line != nullptr);
    
    char * curr_char = line;
    while (isspace(*curr_char))
        curr_char++;
        
    return curr_char;
}

Node* ReadNode(char** ptr, Tree *tree)
{
    char * begin = strchr(*ptr, '\"');
    char * end   = strchr(begin + 1, '\"');
    *ptr = end + 1;
    
    char *tmp = GetNextChar(*ptr);
    
    if (*tmp == '\"')
        *ptr = tmp;
    else
        *ptr = tmp + 1;
        
    struct Node* curr_node = nullptr;
    
    if (*tmp == '[')
    {
        curr_node = ReadMakeNode(0, begin, end, tree);
        assert(curr_node != nullptr);
        curr_node->left  = ReadNode(ptr, tree);
        curr_node->right = ReadNode(ptr, tree);
        return curr_node;
    }
    else if (*tmp == '\"' || *tmp == ']')
    {
        curr_node = ReadMakeNode(1, begin, end, tree);
        assert(curr_node != nullptr);
        return curr_node;
    }
    else
    {
        printf("<%c>\n", *tmp);
        printf("Sorry, wrong database!\n");
        return nullptr;
    }
}

void ReadDatabase(Tree* tree, const char* database_file)
{
    assert(database_file != nullptr);
    assert(tree != nullptr);
    
    FILE *database = fopen(database_file, "rb");
    
    int file_size = FileSize(database);
    fseek(database, 0, SEEK_SET);
    
    char *database_line = (char *)calloc(file_size, sizeof(char));
    fread(database_line, file_size, sizeof(char), database);
    fclose(database);
    
    char * ptr = database_line;
    
    tree->root = ReadNode(&ptr, tree);
    
    free(database_line);
}

void PrintNode(Node *node, FILE* new_database, int offset)
{
    assert(new_database != nullptr);
    
    if (node == nullptr)
        return;
    
    if (node->ans == 0)
    {
        if (offset == 0)
        {
            fprintf(new_database, "\"%s\"\n", node->line);
            fprintf(new_database, "[\n");
        }
        else
        {
            fprintf(new_database, "%*c\"%s\"\n", offset, ' ', node->line);
            fprintf(new_database, "%*c[\n", offset, ' ');
        }
        PrintNode(node->left, new_database, offset + 4);
        PrintNode(node->right, new_database, offset + 4);
        if (offset == 0)
            fprintf(new_database, "]\n");
        else
            fprintf(new_database, "%*c]\n", offset, ' ');
    }
    else
    {
        fprintf(new_database, "%*c\"%s\"\n", offset, ' ', node->line);
        return;
    }
    
}

void PrintDatabase(Tree *tree)
{
    assert(tree != nullptr);
    
    FILE * new_database = fopen("new_database.db", "wb");
    
    PrintNode(tree->root, new_database, 0);
    
    fclose(new_database);
    
    printf("Done! Database was updated\n");
}
