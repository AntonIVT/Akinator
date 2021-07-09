#include "attribute.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void GetAttributes(Tree *tree)
{
    char* word = GetLineAlloc("Print thing which attribute you want to check");
    Label *labels = (Label *)calloc(MAX_WAY, sizeof(Label));
    
    if (GetWay(labels, 0, word, tree->root) == true)
    {
        printf("Got it!! Here's %s's attributes:\n\n", word);
        
        if (labels[0].isTrue == true)
            SayAndPrint(tree->voice, "It ");
        else
            SayAndPrint(tree->voice, "It not ");
            
        SayAndPrint(tree->voice, "%s", labels[0].pointer_to_qestion);
        
        for (int i = 1; labels[i].pointer_to_qestion != nullptr; i++)
        {
            if (labels[i].isTrue == false)
                SayAndPrint(0,", not ");
            else
                printf(", ");
            
                
            SayAndPrint(tree->voice, "%s", labels[i].pointer_to_qestion);
            
        }
        
        printf("\n");
    }
    else
    {
        SayAndPrint(tree->voice, "Sorry I can't find this thing :(\n");
    }
    
    free(labels);
    free(word);
}

//true if word, false if not found
bool GetWay(Label *labels, int free, char * word, Node *node)
{
    if (free >= MAX_LINE - 2)
    {
        printf("Labels overflow\n");
        return false;
    }
    
    assert(labels != nullptr);
    assert(word != nullptr);
    
    if (node == nullptr)
        return false;
    
    if (node->ans == 0)
    {
        labels[free] = {node->line, false};
        
        char tmp = GetWay(labels, free + 1, word, node->left);
        
        if (tmp == true)
        {
            labels[free].isTrue = true;
            return true;
        }
            
        tmp = GetWay(labels, free + 1, word, node->right);
        if (tmp == true)
        {
            labels[free].isTrue = false;
            return true;
        }
        else
        {
            labels[free].pointer_to_qestion = nullptr;
            return false;
        }
    }
    else //ans_ == 1
    {
        if (!strcmp(word, node->line))
            return true;
        else
            return false;
    }
}

void FreeDifference(char * word1, char * word2, Label *labels1, Label *labels2)
{
    if (word1 != nullptr)
        free(word1);
    if (word2 != nullptr)
        free(word2);
    if (labels1 != nullptr)
        free(labels1);
    if (labels2 != nullptr)
        free(labels2);
}

void GetDifference(Tree *tree)
{
    assert (tree != nullptr);
    
    printf("Print two things, between which you want to find difference\n");
    
    char* word1 = GetLineAlloc("First thing: ");
    Label *labels1 = (Label *)calloc(MAX_WAY, sizeof(Label));
    
    char* word2 = GetLineAlloc("Second thing: ");
    Label *labels2 = (Label *)calloc(MAX_WAY, sizeof(Label));
    
    if (GetWay(labels1, 0, word1, tree->root) == false)
    {
        printf("Sorry can't find %s thing :(\n", word1);
        FreeDifference(word1, word2, labels1, labels2);
        return;
    }
    if (GetWay(labels2, 0, word2, tree->root) == false)
    {
        printf("Sorry can't find %s thing :(\n", word2);
        FreeDifference(word1, word2, labels1, labels2);
        return;
    }
    
    int index1 = 0,
        index2 = 0;
    
    if (labels1[index1].isTrue == labels2[index2].isTrue)
    {
        SayAndPrint(tree->voice, "\nHere's the same attributes:\nBoth things ");
        
        if (labels1[index1].isTrue == false)
            SayAndPrint(tree->voice, "not ");
            
        SayAndPrint(tree->voice, "%s", labels1[index1].pointer_to_qestion);
        
        index1++;
        index2++;
        
        if (labels1[index1].pointer_to_qestion == nullptr)
        {
            SayAndPrint(tree->voice, "\n\nGoddamn, your this things are the same!\n");
            FreeDifference(word1, word2, labels1, labels2);
            return;
        }
        
        while(labels1[index1].isTrue == labels2[index2].isTrue)
        {
            if (labels1[index1].isTrue == false)
                SayAndPrint(tree->voice, ", not ");
            else
                printf(", ");
            
            SayAndPrint(tree->voice, "%s", labels1[index1].pointer_to_qestion);
          
            index1++;
            index2++;
            
            if (labels1[index1].pointer_to_qestion == nullptr)
            {
                SayAndPrint(tree->voice, "\n\nGoddamn, your this things are the same!\n");
                FreeDifference(word1, word2, labels1, labels2);
                return;
            }
        }
    }
    
    SayAndPrint(tree->voice, "\n\nHere's difference:\n");
    
    if (labels1[index1].pointer_to_qestion != nullptr)
    {
        SayAndPrint(tree->voice, "First thing ");
        
        if (labels1[index1].isTrue == false)
            SayAndPrint( tree->voice, "not ");
        
        SayAndPrint(tree->voice, "%s", labels1[index1].pointer_to_qestion);
        
        index1++;
        
        while(labels1[index1].pointer_to_qestion != nullptr)
        {
            if (labels1[index1].isTrue == false)
                SayAndPrint(tree->voice, ", not ");
            else
                printf(", ");
            
            SayAndPrint(tree->voice, "%s",labels1[index1].pointer_to_qestion);
            
            index1++;
        }
    }
    
    if (labels2[index2].pointer_to_qestion != nullptr)
    {
        SayAndPrint(tree->voice, "\nSecond thing ");
        
        if (labels2[index2].isTrue == false)
            SayAndPrint(tree->voice, "not ");
        
        SayAndPrint(tree->voice, "%s", labels2[index2].pointer_to_qestion);
        
        index2++;
        
        while(labels2[index2].pointer_to_qestion != nullptr)
        {
            if (labels2[index2].isTrue == false)
                SayAndPrint(tree->voice, ", not ");
            else
                printf(", ");
            
            SayAndPrint(tree->voice, "%s" ,labels2[index2].pointer_to_qestion);
            
            index2++;
        }
    }
    printf("\n\n");
    
    FreeDifference(word1, word2, labels1, labels2);
}
