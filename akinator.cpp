//#define WIN

#ifdef WIN
#define TX_USE_SPEAK
#include "TXLib.h-NEW"
#endif

#include "akinator.hpp"

int main()
{    
    struct Tree tree = {0};
           
    Construct(&tree);
        
    #ifdef WIN
    if (get_answer("Do you want to listen my pleasure voice?", 0, 0) == 1)
        tree.voice = true;
    #endif
    
    say_n_print(tree.voice, "Hello!\n");
    
    if (get_answer("Do you want to read database?", 0, 0) == 1)
        read_database(&tree, "new_database.db");
    
    while(Menu(&tree) == 1) {}
    
    Destruct(&tree);
}