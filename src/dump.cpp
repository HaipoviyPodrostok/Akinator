#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>

#include "tree.h"

tree_err_t tree_dump(tree_t* tree, call_cnt_t* call_cnt) {
    
    if (!tree)     return TREE_ERR_TREE_DOES_NOT_EXIST;
    if (!call_cnt) return TREE_ERR_NULL_PTR_ERROR;
        
    call_cnt->call_num++;
    
    system("mkdir -p Text_dumps");
    FILE* counter_file = fopen(COUNTER_FILENAME, "r");
    if (!counter_file) return TREE_ERR_FILE_OPEN_ERROR;
    if (fscanf(counter_file, "Last launch_num = %lu", &call_cnt->launch_num) != 1) return TREE_ERR_FILE_READING_ERROR;

    char*  dot_filename = NULL;
    ERROR_HANDLE(make_dot_filename(&dot_filename, call_cnt));

    FILE* dot_file = fopen(dot_filename, "w");
    str_dtor(dot_filename);
    if (!dot_file) return TREE_ERR_FILE_OPEN_ERROR;
    
    if (tree->root == NULL) {
        fprintf (dot_file, NODE_OF_EMPTY_TREE);
        return TREE_ERR_SUCCESS;
    }
    
    if (fprintf(dot_file, DOT_TITLE) != strlen(DOT_TITLE)) return TREE_ERR_FILE_PRINT_ERROR;

    ERROR_HANDLE(dump_recursion(tree->root, dot_file, tree));

    fprintf(dot_file, "}");
    if (fclose(dot_file) != 0)     return TREE_ERR_FILE_CLOSE_ERROR;
    if (fclose(counter_file) != 0) return TREE_ERR_FILE_CLOSE_ERROR;

    system("mkdir -p Graphviz_dumps");
    char system_command[100] = "";
    sprintf(system_command, SYSTEM_COMMAND_TEMPLATE, call_cnt->launch_num, call_cnt->call_num, call_cnt->launch_num, call_cnt->call_num);
    system(system_command);

    printf("Графический дамп сохранен в папке " GRAPHIZ_DUMPS_FOLDER);

    return TREE_ERR_SUCCESS;
}

tree_err_t dump_recursion(node_t* current, FILE* dot_file, tree_t* tree) {
    
    assert(tree != NULL);        
    assert(current != NULL);
    assert(dot_file != NULL);

    if (current->left)    ERROR_HANDLE(dump_recursion(current->left, dot_file, tree));
    if (current->right)   ERROR_HANDLE(dump_recursion(current->right, dot_file, tree));
      
    ERROR_HANDLE(print_node(current, dot_file, tree));

    return TREE_ERR_SUCCESS;
}

tree_err_t print_node(node_t* current, FILE* dot_file, tree_t* tree) {
    
    assert(tree);    
    assert(current);
    assert(dot_file);

    char fillcolor [20] = "";
    
    if (current == tree->root)    sprintf(fillcolor, ROOT_COLOR);
    else                          sprintf(fillcolor, NODE_COLOR);

    fprintf(dot_file, "\n    node%p [fillcolor= \"%s\", label=\"{<f0> value: %s | { <f1> left: %p |<f2> right: %p }}\"]\n", current, fillcolor, current->str, current->left, current->right);

    if (current->left)    fprintf(dot_file, "    node%p:f1 -> node%p:f0 [label=\"Да\", fontcolor= \""LEFT_ARROW_COLOR"\", "
                                            "color= \""LEFT_ARROW_COLOR"\", penwidth=2]\n", current, current->left);
    if (current->right)   fprintf(dot_file, "    node%p:f2 -> node%p:f0 [label=\"Нет\", fontcolor= \""RIGHT_ARROW_COLOR"\", "
                                            "color= \""RIGHT_ARROW_COLOR"\", penwidth=2]\n", current, current->right);

    return TREE_ERR_SUCCESS;
}

tree_err_t make_dot_filename(char** dot_filename, call_cnt_t* call_cnt) {
    
    size_t dot_filename_size = MAX_STR_SIZE;
    
    *dot_filename = (char*) calloc(dot_filename_size, sizeof(char)); 
    if (!(*dot_filename)) return TREE_ERR_ALLOCATION_ERROR;

    if (snprintf(*dot_filename, dot_filename_size, DOT_FILENAME, call_cnt->launch_num, call_cnt->call_num) >= (int)dot_filename_size) {
        dot_filename_size *= 2;       
        *dot_filename = NULL;
        *dot_filename = (char*) realloc(*dot_filename, dot_filename_size);
        if (!(*dot_filename)) return TREE_ERR_ALLOCATION_ERROR;
        
        snprintf(*dot_filename, dot_filename_size, DOT_FILENAME, call_cnt->launch_num, call_cnt->call_num);
    }

    return TREE_ERR_SUCCESS;
}