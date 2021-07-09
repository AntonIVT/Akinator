#pragma once

#include "tree.h"
#include <stdio.h>

int FileSize(FILE *file);

Node* ReadMakeNode(char mode, char* begin, char* end, Tree *tree);

char* GetNextChar(char *line);

Node* ReadNode(char** ptr, Tree *tree);

void ReadDatabase(Tree* tree, const char* database_file);

void PrintNode(Node *node, FILE* new_database, int offset);

void PrintDatabase(Tree *tree);
