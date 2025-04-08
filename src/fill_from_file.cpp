#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>

#include "tree.h"

tree_err_t fill_tree_from_file(char* tree_source_filename, tree_t* tree) {
    
    assert(tree_source_filename);
    
    FILE* tree_source = fopen(tree_source_filename, "r");
    if (!tree_source) return TREE_ERR_FILE_OPEN_ERROR;

    if (tree->root == NULL) {      
        char* str = NULL;
        fscanf(tree_source, "%m[^\n]", &str);           

        if (strcmp(str, "{") == 0) {
            STR_DTOR(str);
    
            skip_spaces(tree_source);
            fscanf(tree_source, "%m[^\n]", &str);
            insert_root(tree, str);
            STR_DTOR(str);
        }
        else {
            STR_DTOR(str);
            fprintf(stderr, "Input file is incorrect\n");
            return TREE_ERR_INPUT_FILE_INCORRECT;
        }
    }
    
    skip_spaces(tree_source);

    char* str = NULL;
    fscanf(tree_source, "%m[^\n]", &str);

    if (strcmp(str, "left {") == 0) {
        ERROR_HANDLE(fill_tree_recursion(tree_source, tree->root->left));
    }
    if (strcmp(str, "right {") == 0) {
        ERROR_HANDLE(fill_tree_recursion(tree_source, tree->root->right));
    }
    if (strcmp(str, "}") == 0) {
        printf("Tree has succesfuly written"); 
    }
    return TREE_ERR_SUCCESS;
}

tree_err_t fill_tree_recursion(FILE* tree_source, node_t* current) {

    skip_spaces(tree_source);

    char* str = NULL;
    fscanf(tree_source, "%m[^\n]", &str);

    if (strcmp(str, "left {") == 0) {
        fill_node(tree_source, current, NODE_TYPE_LEFT);
        ERROR_HANDLE(fill_tree_recursion(tree_source, current->left));
    }
    if (strcmp(str, "right {") == 0) {
        fill_node(tree_source, current, NODE_TYPE_RIGHT);
        ERROR_HANDLE(fill_tree_recursion(tree_source, current->right));
    }
    if (strcmp(str, "}") == 0) {
        return TREE_ERR_SUCCESS;
    }
    else {
        return TREE_ERR_INPUT_FILE_INCORRECT;
    }
}

void skip_spaces(FILE* tree_source){
    fscanf(tree_source, "%*[ \t\n]");
    return;
}

tree_err_t fill_node(FILE* tree_source, node_t* current, node_type_t node_type) {
    char* str = NULL;
    skip_spaces(tree_source);
    fscanf(tree_source, "%m[^\n]", &str);

    if (node_type == NODE_TYPE_LEFT) {
        ERROR_HANDLE(insert_left(current, str));
        STR_DTOR(str);
    }
    else if (node_type == NODE_TYPE_RIGHT) {
        ERROR_HANDLE(insert_right(current, str));
        STR_DTOR(str);
    }
    else {
        STR_DTOR(str);
        return TREE_ERR_FILL_NODE_ERROR;
    }
    return TREE_ERR_SUCCESS;
}
