#include "tree.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

void TreeConstruct(Tree* that)
{
    assert(that != nullptr);
    
    that->root   = nullptr;
    BufferConstruct(&that->buffer);
        
    that->voice = false;
}

void TreeDestruct(Tree* that)
{
    assert(that != nullptr);
    
    NodeDestruct(that->root);
    BufferDecostruct(&that->buffer);
}

Node *MakeNode(char mode, const char* line)
{
    assert(line != nullptr);
    
    struct Node *node = (struct Node*)calloc(1, sizeof(struct Node));
    
    node->left  = nullptr;
    node->right = nullptr;
    
    node->line = line;
    node->ans = mode;
    
    return node;
}

Node *GetNewRoot(Tree *tree)
{    
    assert(tree != nullptr);
    
    Node *root = MakeNode(0, "is alive");
    
    root->left  = MakeNode(1, "Ded");
    root->right = MakeNode(1, "Lamp");
    
    return root;
}

void PutNewNode(Node *node, Tree *tree)
{
    assert(node != nullptr);
    
    node->right = MakeNode(1, node->line);
    
    char *new_answer = GetLine("What you wondered?", tree);
    char *tmp = AddNewLine((const char *)new_answer, tree);
    node->left = MakeNode(1, tmp);
    
    char *new_question = GetLine("What attribute does your object have?", tree);
    node->line = AddNewLine((const char *)new_question, tree);
    node->ans  = 0;
}

void NodeDestruct(Node *that)
{
    if (that == nullptr)
        return;
    
    NodeDestruct(that->left);
    NodeDestruct(that->right);
    free(that);

    return;
}

char GetAnswer(const char *question, char counter, char mode)
{
    assert(question != nullptr);
    
    if (mode == 1)
    {
        printf("Thing you wondered %s?\n>", question);
    }
    else if (mode == 0)
    {
        printf("%s[yes/no]\n>", question);
    }
    else
        printf("Internal error, sorry\n");
    
    char answer[MAX_LINE] = {0};
    
    fgets(answer, MAX_LINE, stdin);
    strtok(answer, "\n");
    
    if (!strcmp(answer, "yes") || !strcmp(answer, "YES") || !strcmp(answer, "Yes"))
        return 1;
    else if (!strcmp(answer, "no") || !strcmp(answer, "NO") || !strcmp(answer, "No"))
        return 0;
    else
    {
        if (counter < 5)
        {
            printf("Try again, wrong answer\n");
            return GetAnswer(question, ++counter, mode);
        }
        else if (counter < 10)
        {
            printf("Are you stupid or what? Let's again\n");
            return GetAnswer(question, ++counter, mode);
        }
        else if (counter < 15)
        {
            printf("DEBIL. But I'll give you the the last chance\n");
            return GetAnswer(question, ++counter, mode); 
        }
        else
        {    
            printf("No way. Your answer is NO, and that's it\n");
            return 0;
        }
    }
}

char* GetLine(const char *line, Tree *tree)
{
    assert(line != nullptr);
    
    printf("%s\n>", line);
    
    fgets(tree->read_buffer, MAX_LINE, stdin);
    strtok(tree->read_buffer, "\n");
        
    return tree->read_buffer;
}

char* GetLineAlloc(const char *line)
{
    assert(line != nullptr);
    
    printf("%s\n>", line);
    
    char *question = (char *)calloc(MAX_LINE, sizeof(char));
    
    fgets(question, MAX_LINE, stdin);
    strtok(question, "\n");
        
    return question;
}

char* AddNewLine(const char *new_line, Tree *tree)
{
    assert(new_line != nullptr);
    assert(tree != nullptr);
    
    size_t length = strlen(new_line);
    int curr_buffer = tree->buffer.curr;
    
    if ((size_t)DEFAULT_CAPACITY + (size_t)tree->buffer.buffers[curr_buffer] - (size_t)tree->buffer.free > length)
    {
        strncpy(tree->buffer.free, new_line, length + 1);
        
        tree->buffer.free += length + 1;
    
        return (tree->buffer.free - (length + 1));
    }
    else
    {   
        if (curr_buffer == tree->buffer.count - 1)
            BufferResize(&tree->buffer);
                
        tree->buffer.curr++;
        tree->buffer.free = tree->buffer.buffers[++curr_buffer];
        
        return AddNewLine(new_line, tree);
    }
}

void Quest(Tree *tree)
{
    assert(tree != nullptr);
    
    if (tree->root == nullptr)
    {
        printf("No root, sorry\n");
        return;
    }
    
    Node *that = tree->root;
    
    while (that != nullptr)
    {
        if (that->ans == 0)                        // Attribute
        {
            if (GetAnswer(that->line, 0, 1) == 1) // Answer : Yes
                that = that->left;
            else                                    // Answer : No
                that = that->right;
        }
        else                                        // Answer
        {
            if (GetAnswer(that->line, 0, 1) == 1)
            {
                printf("I am absolutely right, as I told\n");
                return;
            }
            else
            {
                printf("I'm sorry, I wasn't right. But I can be better with your help\n");
                            
                if (GetAnswer("Could you help me?", 0, 0) == 1)
                {
                    printf("AWESOME!\n");
                    PutNewNode(that, tree);
                    return;
                }
                else
                {
                    printf("That's OK\n");
                    return;
                }
            }
        }
    }
}

void SayAndPrint(bool isVoice, const char *format, ...)
{
    char buffer[300] = {0};
    char sentence_to_say[300] = {0};
    
    va_list arg;
    va_start(arg, format);
    vsprintf(buffer, format, arg);
    va_end(arg);
    
    #ifdef WIN
    
    if (isVoice == true)
    {    
        strcpy(sentence_to_say, "<speak version='1.0' xmlns='http://www.w3.org/2001/10/synthesis' xml:lang='EN'> ");
        strcat(sentence_to_say, buffer);
        strcat(sentence_to_say, " </speak>");
        txSpeak("%s", sentence_to_say);
    }
    #endif
    
    
    printf("%s", buffer);
}
