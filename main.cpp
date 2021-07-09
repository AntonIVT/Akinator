/* Windows compile support voice manager */
#ifdef WIN
#define TX_USE_SPEAK
#include "TXLib.h"
#endif

#include "src/tree.h"
#include "src/graph.h"
#include "src/database.h"
#include "src/attribute.h"
#include <assert.h>
#include <string.h>

char Menu(Tree *tree)
{   
    assert(tree != nullptr);
    
    if (tree->root == nullptr)
        tree->root = GetNewRoot(tree);
    
    char answer[MAX_LINE + 1] = {0};
    
    printf("\n*********MENU*********\n"
           "What do you want to do?\n"
           "- To play the game press      [1]\n"
           "- To check attributes press   [2]\n"
           "- To check difference press   [3]\n"
           "- To check all tree press     [4]\n"
           "- To print new database press [5]\n"
           "- To exit                     [6]\n"
           ">");
    
    fgets(answer, MAX_LINE, stdin);
    
    if      (!strcmp(answer, "1\n"))
        Quest(tree);
    else if (!strcmp(answer, "2\n"))
        GetAttributes(tree);
    else if (!strcmp(answer, "3\n"))
        GetDifference(tree);
    else if (!strcmp(answer, "4\n"))
        GetGraph(tree);
    else if (!strcmp(answer, "5\n"))
        PrintDatabase(tree);
    else if (!strcmp(answer, "6\n"))
        return 0;
    else
    {
        printf("Wrong enter type\n");
        return 1;
    }

    if (GetAnswer("You want to go back to menu?", 0, 0) == 1)
        return 1;
    else
        return 0;
}

int main()
{    
    struct Tree tree = {0};
           
    TreeConstruct(&tree);
        
    #ifdef WIN
    if (get_answer("Do you want to listen my pleasure voice?", 0, 0) == 1)
        tree.voice = true;
    #endif
    
    SayAndPrint(tree.voice, "Hello!\n");
    
    if (GetAnswer("Do you want to read database(\"database.db\")?", 0, 0) == 1)
        ReadDatabase(&tree, "database.db");
    
    while(Menu(&tree) == 1) {}
    
    TreeDestruct(&tree);
}
