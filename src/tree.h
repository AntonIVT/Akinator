#pragma once

#include "buffer.h"

/* Max length of line to read */
const int MAX_LINE         = 100;
/* Max deep of tree to check difference */
const int MAX_WAY          = 1000;

struct Node
{
    const char * line;
    char ans;                // 1 if ans, 0 if question
    char buf;
    Node *left;
    Node *right;
};

struct Tree
{
    Node *root;
    Buffer buffer;
    char read_buffer[MAX_LINE];
    bool voice;
};

void TreeConstruct(Tree* that);

void TreeDestruct(Tree* that);

/* Make node with allocation */
Node* MakeNode(char mode, const char* line);

/* Make start root (without database reading) */
Node* GetNewRoot(Tree *tree);

/* Put new node to tree from user */
void PutNewNode(Node *that, Tree *tree);

void NodeDestruct(Node *that);

/* Returning 1 if true, 0 if else */
char GetAnswer(const char *question, char counter, char mode);

/* Get input line to tree->read_buffer */
char* GetLine(const char *line, Tree *tree);

/* Get input line to allocated memory */
char* GetLineAlloc(const char *line);

/* Add new_line to buffer in tree */
char* AddNewLine(const char *new_line, Tree *tree);

/* Quest game Akinator */
void Quest(Tree *tree);

void SayAndPrint(bool isVoice, const char *format, ...);
