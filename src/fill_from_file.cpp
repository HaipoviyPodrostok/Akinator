#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>

#include "tree.h"

tree_err_t fill_tree_from_file(FILE* tree_source, tree_t* tree, call_cnt_t* call_cnt) {
    
    assert(tree_source);

    if (tree->root == NULL) {      
        char* str = NULL;
        fscanf(tree_source, "%m[^\n]", &str);           

        if (strcmp(str, "{") == 0) {
            str_dtor(str);
            ERROR_HANDLE(fill_node(tree_source, &(tree->root), NODE_TYPE_ROOT));
        } else {
            str_dtor(str);
            fprintf(stderr, "Input file is incorrect\n");
            return TREE_ERR_INPUT_FILE_INCORRECT;
        }
    }
    
    char* str = NULL;
    skip_spaces(tree_source);
    fscanf(tree_source, "%m[^\n]", &str);

    if (strcmp(str, "left {") == 0) {
        str_dtor(str);
        ERROR_HANDLE(fill_tree_recursion(tree, tree_source, tree->root, NODE_TYPE_LEFT, call_cnt));
        skip_spaces(tree_source);
        fscanf(tree_source, "%m[^\n]", &str);
    }
    if (strcmp(str, "right {") == 0) {
        str_dtor(str);
        ERROR_HANDLE(fill_tree_recursion(tree, tree_source, tree->root, NODE_TYPE_RIGHT, call_cnt));
        skip_spaces(tree_source);
        fscanf(tree_source, "%m[^\n]", &str);
    }
    if (strcmp(str, "}") == 0) {
        str_dtor(str);
        printf(CYAN "Дерево успешно заполнено!\n" RESET); 
        return TREE_ERR_SUCCESS;
    }
    else {
        str_dtor(str);
        return TREE_ERR_INPUT_FILE_INCORRECT;
    }
    return TREE_ERR_INPUT_FILE_INCORRECT;
}

tree_err_t fill_tree_recursion(tree_t* tree, FILE* tree_source, node_t* current, node_type_t node_type, call_cnt_t* call_cnt) {
    assert(tree_source);
    assert(current);

    ERROR_HANDLE(fill_node(tree_source, &current, node_type));
    //current = current->left/right;

    char* str = NULL;
    skip_spaces(tree_source);
    fscanf(tree_source, "%m[^\n]", &str);

    if (strcmp(str, "left {") == 0) { 
        str_dtor(str);
        ERROR_HANDLE(fill_tree_recursion(tree, tree_source, current, NODE_TYPE_LEFT, call_cnt));
        skip_spaces(tree_source);
        fscanf(tree_source, "%m[^\n]", &str);
    }
    if (strcmp(str, "right {") == 0) {
        str_dtor(str);
        ERROR_HANDLE(fill_tree_recursion(tree, tree_source, current, NODE_TYPE_RIGHT, call_cnt));
        skip_spaces(tree_source);
        fscanf(tree_source, "%m[^\n]", &str);
    }
    if (strcmp(str, "}") == 0) {  
        str_dtor(str);
        return TREE_ERR_SUCCESS;
    }
    else {
        str_dtor(str);
        return TREE_ERR_INPUT_FILE_INCORRECT;
    }
    return TREE_ERR_INPUT_FILE_INCORRECT;
}

tree_err_t fill_node(FILE* tree_source, node_t** current, node_type_t node_type) {   
    assert(tree_source);
    assert(current);
    
    char* str = NULL;
    skip_spaces(tree_source);
    fscanf(tree_source, "%m[^\n]", &str);

    if (node_type == NODE_TYPE_LEFT) {
        ERROR_HANDLE(insert_left(*current, str));
        *current = (*current)->left;
        str_dtor(str);
    } else if (node_type == NODE_TYPE_RIGHT) {
        ERROR_HANDLE(insert_right(*current, str));
        *current = (*current)->right;
        str_dtor(str);
    } else if (node_type == NODE_TYPE_ROOT) {
        ERROR_HANDLE(insert_root(current, str));
        str_dtor(str);
    }
    else {
        str_dtor(str);
        return TREE_ERR_FILL_NODE_ERROR;
    }
    return TREE_ERR_SUCCESS;
}
