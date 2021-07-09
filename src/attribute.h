#pragma once

#include "tree.h"

struct Label
{
    const char *pointer_to_qestion;
    bool isTrue;   
};

void GetAttributes(Tree *tree);

bool GetWay(Label* labels, int free, char* word, Node* node);

void FreeDifference(char* word1, char* word2, Label* labels1, Label* labels2);

void GetDifference(Tree* tree);
