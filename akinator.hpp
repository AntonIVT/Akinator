#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <cstdarg>

const int MAX_LINE = 100;
const int MAX_WAY = 1000;
const int DEFAULT_CAPACITY = 1000;

//------------------------------------------- Structures and Prototypes --------------------------------------------\\
//==================================================================================================================\\

struct Node
{
    const char * line;
    char ans;                // 1 if ans, 0 if question
    char buf;
    Node *left;
    Node *right;
};

struct Buffer
{
    char **buffers;
    int count;
    int curr;
    char *free;
};

struct Tree
{
    Node *root;
    Buffer buffer;
    char *buffer_tmp;
    bool voice;
};

struct Label
{
    const char *pointer_to_qestion;
    bool isTrue;   
};

//==================================================

void Construct(Tree* that);

void Destruct(Tree* that);

char Menu(Tree *tree);

//==================================================

void bufferConstruct(Buffer *buff);

void bufferResize(Buffer *buff);

void bufferDecostruct(Buffer *buff);

void destruct_node(struct Node *that);

char get_answer(const char *question, char counter, char mode);

char* get_line(const char *line, Tree *tree);

char *get_line_with_allocation(const char *line);

Node *make_node(char mode, const char* line);

char * add_new_line(const char *new_line, Tree *tree);

Node *new_root(Tree *tree);

void new_node(Node *that, Tree *tree);

void quest(Tree *tree);

void say_n_print(bool isVoice, const char *format, ...);

//--------------------------------------------------

int FileSize(FILE *file);

Node* read_n_make_node(char mode, char* begin, char* end, Tree *tree);

char * get_next_char(char *line);

Node* read_node(char** ptr, Tree *tree);

void read_database(Tree* tree, const char* database_file);

void print_node(Node *node, FILE* new_database, int offset);

void print_database(Tree *tree);

//--------------------------------------------------

void graph_node(FILE* graph_file, Node *node);

void get_graph(Tree *tree);

//--------------------------------------------------

void get_attributes(Tree *tree);

bool get_way(Label *labels, int free, char * word, Node *node);

//--------------------------------------------------

void free_difference_stuff(char * word1, char * word2, Label *labels1, Label *labels2);

void get_difference(Tree *tree);

//--------------------------------------------------- Tree stuff ------------------------------------------------------\\
//=====================================================================================================================\\

void Construct(Tree* that)
{
    assert(that != nullptr);
    
    that->root   = nullptr;
    bufferConstruct(&that->buffer);
    
    that->buffer_tmp = (char *)calloc(MAX_LINE, sizeof(char));
    
    that->voice = false;
}

void bufferConstruct(Buffer *buff)
{
    assert(buff != nullptr);
    
    buff->count = 1;
    buff->curr = 0;
    
    buff->buffers = (char **)calloc(buff->count, sizeof(Buffer));
    
    for (int i = 0; i < buff->count; i++)
        buff->buffers[i] = (char*)calloc(DEFAULT_CAPACITY + 1, sizeof(char));
    
    buff->free = buff->buffers[0];    
}

void bufferResize(Buffer *buff)
{
    assert(buff != nullptr);
    
    buff->buffers = (char **)realloc(buff->buffers, sizeof(Buffer) * buff->count * 2);
    
    for (int i = buff->count; i < buff->count * 2; i++)
        buff->buffers[i] = (char *)calloc(DEFAULT_CAPACITY, sizeof(char));
    
    buff->count *= 2;
}

void bufferDecostruct(Buffer *buff)
{
    assert(buff != nullptr);
        
    for(int i = 0; i < buff->count; i++)
    {
        if (buff->buffers[i] != nullptr)
            free(buff->buffers[i]);
    }
    free(buff->buffers);
    
    buff->count = 0;
    buff->free = nullptr;
    buff->curr = -1;
}

void destruct_node(Node *that)
{
    if (that == nullptr)
        return;
    
    destruct_node(that->left);
    destruct_node(that->right);
    free(that);
    return;
}

void Destruct(Tree* that)
{
    assert(that != nullptr);
    
    destruct_node(that->root);
    bufferDecostruct(&that->buffer);
    
    free(that->buffer_tmp);
}

// mode == 0 if there's all question in char*(without \n), and 1 if you need prefix. Counter is 0 always
char get_answer(const char *question, char counter, char mode)
{
    assert(question != nullptr);
    
    if (mode == 1)
    {
        printf("Thing you wondered %s?\n>", question);
    }
    else if (mode == 0)
    {
        printf("%s\n>", question);
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
            return get_answer(question, ++counter, mode);
        }
        else if (counter < 10)
        {
            printf("Are you stupid or what? Let's again\n");
            return get_answer(question, ++counter, mode);
        }
        else if (counter < 15)
        {
            printf("DEBIL. But I'll give you the the last chance\n");
            return get_answer(question, ++counter, mode); 
        }
        else
        {    
            printf("No way. Your answer is NO, and that's it\n");
            return 0;
        }
    }
}

// Maximum 100 symbols
char* get_line(const char *line, Tree *tree)
{
    assert(line != nullptr);
    
    printf("%s\n>", line);
    
    fgets(tree->buffer_tmp, MAX_LINE, stdin);
    strtok(tree->buffer_tmp, "\n");
        
    return tree->buffer_tmp;
}

char *get_line_with_allocation(const char *line)
{
    assert(line != nullptr);
    
    printf("%s\n>", line);
    
    char *question = (char *)calloc(MAX_LINE, sizeof(char));
    
    fgets(question, MAX_LINE, stdin);
    strtok(question, "\n");
        
    return question;
}

// mode = 1, if ans, mode = 0, if question
Node *make_node(char mode, const char* line)
{
    assert(line != nullptr);
    
    struct Node *node = (struct Node*)calloc(1, sizeof(struct Node));
    
    node->left  = nullptr;
    node->right = nullptr;
    
    node->line = line;
    node->ans = mode;
    
    return node;
}

char * add_new_line(const char *new_line, Tree *tree)
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
            bufferResize(&tree->buffer);
                
        tree->buffer.curr++;
        tree->buffer.free = tree->buffer.buffers[++curr_buffer];
        
        return add_new_line(new_line, tree);
    }
}

Node *new_root(Tree *tree)
{    
    assert(tree != nullptr);
    
    Node *root = make_node(0, "is alive");
    
    root->left  = make_node(1, "Ded");
    root->right = make_node(1, "Lamp");
    
    return root;
}

void new_node(struct Node *that, Tree *tree)
{
    assert(that != nullptr);
    
    that->right = make_node(1, that->line);
    
    char *new_answer = get_line("What you wondered?", tree);
    char *tmp = add_new_line((const char *)new_answer, tree);
    that->left = make_node(1, tmp);
    
    char *new_question = get_line("What attribute does your object have?", tree);
    that->line = add_new_line((const char *)new_question, tree);
    that->ans  = 0;
}

void quest(Tree *tree)
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
            if (get_answer(that->line, 0, 1) == 1) // Answer : Yes
                that = that->left;
            else                                    // Answer : No
                that = that->right;
        }
        else                                        // Answer
        {
            if (get_answer(that->line, 0, 1) == 1)
            {
                printf("I am absolutely right, as I told\n");
                return;
            }
            else
            {
                printf("I'm sorry, I wasn't right. But I can be better with your help\n");
                            
                if (get_answer("Could you help me?", 0, 0) == 1)
                {
                    printf("AWESOME!\n");
                    new_node(that, tree);
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

// if you want to exit than 0
char Menu(Tree *tree)
{   
    assert(tree != nullptr);
    
    if (tree->root == nullptr)
        tree->root = new_root(tree);
    
    char answer[MAX_LINE + 1] = {0};
    
    printf("\n*********MENU*********\n"
           "What do you want to do?\n"
           "- To play the game press      [1]\n"
           "- To check attributes press   [2]\n"
           "- To check difference press   [3]\n"
           "- To check all tree press     [4]\n"
           "- To print new database press [5]\n>");
    
    fgets(answer, MAX_LINE, stdin);
    
    if      (!strcmp(answer, "1\n"))
        quest(tree);
    else if (!strcmp(answer, "2\n"))
        get_attributes(tree);
    else if (!strcmp(answer, "3\n"))
        get_difference(tree);
    else if (!strcmp(answer, "4\n"))
        get_graph(tree);
    else if (!strcmp(answer, "5\n"))
        print_database(tree);
    else
    {
        printf("Wrong enter type\n");
        return 1;
    }

    if (get_answer("You want to go back to menu?", 0, 0) == 1)
        return 1;
    else
        return 0;
}
   
//--------------------------------------------------- Database stuff -----------------------------------------------\\
//==================================================================================================================\\

int FileSize(FILE *file)
{
    assert(file != NULL);

	int _file_size = -1;

	fseek(file, 0, SEEK_END);
	_file_size = ftell(file);

	return _file_size;
}

// mode = 1, if ans, mode = 0, if question
Node* read_n_make_node(char mode, char* begin, char* end, Tree *tree)
{
    assert(begin != nullptr);
    assert(end != nullptr);
    assert(end - begin > 0);
    
    *end = 0;
    struct Node *node = make_node(mode, add_new_line(begin + 1, tree));
    *end = '\"';
    
    return node;
}

char * get_next_char(char *line)
{
    assert(line != nullptr);
    
    char * curr_char = line;
    while (isspace(*curr_char))
        curr_char++;
        
    return curr_char;
}

Node* read_node(char** ptr, Tree *tree)
{
    char * begin = strchr(*ptr, '\"');
    char * end   = strchr(begin + 1, '\"');
    *ptr = end + 1;
    
    char *tmp = get_next_char(*ptr);
    
    if (*tmp == '\"')
        *ptr = tmp;
    else
        *ptr = tmp + 1;
        
    struct Node* curr_node = nullptr;
    
    if (*tmp == '[')
    {
        curr_node = read_n_make_node(0, begin, end, tree);
        assert(curr_node != nullptr);
        curr_node->left  = read_node(ptr, tree);
        curr_node->right = read_node(ptr, tree);
        return curr_node;
    }
    else if (*tmp == '\"' || *tmp == ']')
    {
        curr_node = read_n_make_node(1, begin, end, tree);
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

void read_database(Tree* tree, const char* database_file)
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
    
    tree->root = read_node(&ptr, tree);
    
    free(database_line);
}

void print_node(Node *node, FILE* new_database, int offset)
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
        print_node(node->left, new_database, offset + 4);
        print_node(node->right, new_database, offset + 4);
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

void print_database(Tree *tree)
{
    assert(tree != nullptr);
    
    FILE * new_database = fopen("new_database.db", "wb");
    
    print_node(tree->root, new_database, 0);
    
    fclose(new_database);
    
    printf("Done! Database was updated\n");
}

//--------------------------------------------------- GraphViz stuff ---------------------------------------------------\\
//======================================================================================================================\\

void get_graph(Tree *tree)
{
    assert(tree != nullptr);
    
    FILE *graph_file = fopen("Graph.gv", "wb");
    fprintf(graph_file, "digraph structs {\n"
                        "    node [shape=record];\n");
        
    graph_node(graph_file, tree->root);
    
    fprintf(graph_file, "}\n");
    fclose(graph_file);
    
    system("dot -Tpdf Graph.gv -o Graph_Akinator.pdf && rm Graph.gv");
    
    printf("Done! You can check Graph_Akinator.pdf\n");
}

void graph_node(FILE* graph_file, Node *node)
{
    if (node == nullptr)
        return;
    
    fprintf(graph_file, "struct_%p [shape=record, style = filled, fillcolor = \"#8ddfff\", label = \"  %s \"];\n", node, node->line);
        
    if (node->left != nullptr)
    {
        graph_node(graph_file, node->left);
        fprintf(graph_file, "edge [color = \"#ff9d14\"];\n struct_%p->struct_%p [style = bold,label = \"Yes\"];\n", node, node->left);
    }
    if (node->right != nullptr)
    {
        graph_node(graph_file, node->right);
        fprintf(graph_file, "edge [color = \"#ff5533\"];\nstruct_%p->struct_%p [style = bold,label = \"No\"];\n", node, node->right);
    }
    return;
}

void say_n_print(bool isVoice, const char *format, ...)
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

//--------------------------------------------------- Attribute stuff ------------------------------------------------------\\
//==========================================================================================================================\\

void get_attributes(Tree *tree)
{
    char* word = get_line_with_allocation("Print thing which attribute you want to check");
    Label *labels = (Label *)calloc(MAX_WAY, sizeof(Label));
    
    if (get_way(labels, 0, word, tree->root) == true)
    {
        printf("Got it!! Here's %s's attributes:\n\n", word);
        
        if (labels[0].isTrue == true)
            say_n_print(tree->voice, "It ");
        else
            say_n_print(tree->voice, "It not ");
            
        say_n_print(tree->voice, "%s", labels[0].pointer_to_qestion);
        
        for (int i = 1; labels[i].pointer_to_qestion != nullptr; i++)
        {
            if (labels[i].isTrue == false)
                say_n_print(0,", not ");
            else
                printf(", ");
            
                
            say_n_print(tree->voice, "%s", labels[i].pointer_to_qestion);
            
        }
        
        printf("\n");
    }
    else
    {
        say_n_print(tree->voice, "Sorry I can't find this thing :(\n");
    }
    
    free(labels);
    free(word);
}

//true if word, false if not found
bool get_way(Label *labels, int free, char * word, Node *node)
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
        
        char tmp = get_way(labels, free + 1, word, node->left);
        
        if (tmp == true)
        {
            labels[free].isTrue = true;
            return true;
        }
            
        tmp = get_way(labels, free + 1, word, node->right);
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


//--------------------------------------------------- Difference stuff ------------------------------------------------------\\
//===========================================================================================================================\\

void free_difference_stuff(char * word1, char * word2, Label *labels1, Label *labels2)
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

void get_difference(Tree *tree)
{
    assert (tree != nullptr);
    
    printf("Print two things, between which you want to find difference\n");
    
    char* word1 = get_line_with_allocation("First thing: ");
    Label *labels1 = (Label *)calloc(MAX_WAY, sizeof(Label));
    
    char* word2 = get_line_with_allocation("Second thing: ");
    Label *labels2 = (Label *)calloc(MAX_WAY, sizeof(Label));
    
    if (get_way(labels1, 0, word1, tree->root) == false)
    {
        printf("Sorry can't find %s thing :(\n", word1);
        free_difference_stuff(word1, word2, labels1, labels2);
        return;
    }
    if (get_way(labels2, 0, word2, tree->root) == false)
    {
        printf("Sorry can't find %s thing :(\n", word2);
        free_difference_stuff(word1, word2, labels1, labels2);
        return;
    }
    
    int index1 = 0,
        index2 = 0;
    
    if (labels1[index1].isTrue == labels2[index2].isTrue)
    {
        say_n_print(tree->voice, "\nHere's the same attributes:\nBoth things ");
        
        if (labels1[index1].isTrue == false)
            say_n_print(tree->voice, "not ");
            
        say_n_print(tree->voice, "%s", labels1[index1].pointer_to_qestion);
        
        index1++;
        index2++;
        
        if (labels1[index1].pointer_to_qestion == nullptr)
        {
            say_n_print(tree->voice, "\n\nGoddamn, your this things are the same!\n");
            free_difference_stuff(word1, word2, labels1, labels2);
            return;
        }
        
        while(labels1[index1].isTrue == labels2[index2].isTrue)
        {
            if (labels1[index1].isTrue == false)
                say_n_print(tree->voice, ", not ");
            else
                printf(", ");
            
            say_n_print(tree->voice, "%s", labels1[index1].pointer_to_qestion);
          
            index1++;
            index2++;
            
            if (labels1[index1].pointer_to_qestion == nullptr)
            {
                say_n_print(tree->voice, "\n\nGoddamn, your this things are the same!\n");
                free_difference_stuff(word1, word2, labels1, labels2);
                return;
            }
        }
    }
    
    say_n_print(tree->voice, "\n\nHere's difference:\n");
    
    if (labels1[index1].pointer_to_qestion != nullptr)
    {
        say_n_print(tree->voice, "First thing ");
        
        if (labels1[index1].isTrue == false)
            say_n_print( tree->voice, "not ");
        
        say_n_print(tree->voice, "%s", labels1[index1].pointer_to_qestion);
        
        index1++;
        
        while(labels1[index1].pointer_to_qestion != nullptr)
        {
            if (labels1[index1].isTrue == false)
                say_n_print(tree->voice, ", not ");
            else
                printf(", ");
            
            say_n_print(tree->voice, "%s",labels1[index1].pointer_to_qestion);
            
            index1++;
        }
    }
    
    if (labels2[index2].pointer_to_qestion != nullptr)
    {
        say_n_print(tree->voice, "\nSecond thing ");
        
        if (labels2[index2].isTrue == false)
            say_n_print(tree->voice, "not ");
        
        say_n_print(tree->voice, "%s", labels2[index2].pointer_to_qestion);
        
        index2++;
        
        while(labels2[index2].pointer_to_qestion != nullptr)
        {
            if (labels2[index2].isTrue == false)
                say_n_print(tree->voice, ", not ");
            else
                printf(", ");
            
            say_n_print(tree->voice, "%s" ,labels2[index2].pointer_to_qestion);
            
            index2++;
        }
    }
    printf("\n\n");
    
    free_difference_stuff(word1, word2, labels1, labels2);
}






