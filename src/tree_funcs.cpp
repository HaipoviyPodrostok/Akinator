#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>

#include "tree.h"

tree_err_t tree_ctor(tree_t* tree, call_cnt_t* call_cnt) {
    
    assert(tree != NULL);    
    assert(call_cnt != NULL);
    
    //TREE INITIALIZATION:
    tree->root = NULL;
    tree->size = 0;      //TODO size make

    //CALL_COUNTER INITIALIZATION:
    call_cnt->call_num = 0;
    call_cnt->launch_num = 0;

    //PROGRAMM LAUNCH COUNTER:
    FILE* counter_file = fopen(COUNTER_FILENAME, "r+t");

    if (!counter_file) {
        counter_file = fopen(COUNTER_FILENAME, "w+");
        call_cnt->launch_num = 1;
    }
    else {
        if (fscanf(counter_file, "Last launch_num = %lu", &(call_cnt->launch_num)) != 1) {
            return TREE_ERR_DUMP_ERROR;
        }
        call_cnt->launch_num++;
    }

    //PRINT LAUNCH_NUM TO COUNTER FILE:
    fseek(counter_file, 0, SEEK_SET);
    if (fprintf(counter_file, "Last launch_num = %lu", call_cnt->launch_num) < 0) return TREE_ERR_FILE_PRINT_ERROR;
    fclose(counter_file);

    return TREE_ERR_SUCCESS;
}

tree_err_t node_add(tree_t* tree, const char* str) {
    
    assert(tree != NULL);    

    if (tree->root != NULL) {
        ERROR_HANDLE(node_add_recursion(tree->root, str));
    }       
    else {
        //HANDLE ROOT:
        tree->root = (node_t*) calloc(1, sizeof(node_t));
        if (!tree->root) return TREE_ERR_ALLOCATION_ERROR;     

        tree->root->left = NULL;
        tree->root->right = NULL;
        
        tree->root->str = strdup(str);
    }

    return TREE_ERR_SUCCESS;
}

tree_err_t node_add_recursion(node_t* current, const char* str) {
    
    assert(current != NULL);    
    
    if (strcmp(str, current->str) < 0) {
        if (current->left != NULL) {
            ERROR_HANDLE(node_add_recursion(current->left, str));
        } 
        else {
            node_t* new_node = current->left; // = NULL
            new_node = (node_t*) calloc(1, sizeof(node_t));
            if (!new_node) return TREE_ERR_ALLOCATION_ERROR;
            
            new_node->left = NULL;
            new_node->right = NULL;
            
            new_node->str = strdup(str);
        }
    }
    else {
        if (current->right != NULL) {           
            ERROR_HANDLE(node_add_recursion(current->right, str));
        }  
        else {
            node_t* new_node = current->right; // = NULL
            new_node = (node_t*) calloc(1, sizeof(node_t));
            if (!new_node) return TREE_ERR_ALLOCATION_ERROR;
            
            new_node->left = NULL;
            new_node->right = NULL;

            new_node->str = strdup(str);
            if (new_node->str == NULL) return TREE_ERR_ALLOCATION_ERROR;
        }
    }

    return TREE_ERR_SUCCESS;
}

tree_err_t insert_root(node_t** root, char* str) {

    if (*root) {
        fprintf(stderr, "Root is not empty!\n");
        return TREE_ERR_INSERTION_ERROR;
    }

    *root = (node_t*) calloc(1, sizeof(node_t));
    if (!root) return TREE_ERR_ALLOCATION_ERROR;

    (*root)->left  = NULL;
    (*root)->right = NULL; 

    (*root)->str = strdup(str);
    if (!(*root)->str) return TREE_ERR_STR_PRINT_ERROR;

    return TREE_ERR_SUCCESS;
}

tree_err_t insert_left(node_t* current, char* str) {
    
    assert(current != NULL);
    
    if (current->left != NULL) {
        fprintf(stderr, "Left node already exists!\n");
        return TREE_ERR_INSERTION_ERROR;
    }
    
    node_t* new_node = current->left; // = NULL
    new_node = (node_t*) calloc(1, sizeof(node_t));
    if (!new_node) return TREE_ERR_ALLOCATION_ERROR;

    new_node->left = NULL;
    new_node->right = NULL;

    new_node->str = strdup(str);
    if (!new_node->str) return TREE_ERR_ALLOCATION_ERROR;

    current->left = new_node;
    
    return TREE_ERR_SUCCESS;
}

tree_err_t insert_right(node_t* current, char* str) {
    
    assert(current);
    
    if (current->right) {
        fprintf(stderr, "Left node already exists!\n");  // TODO проверить ошибку
        return TREE_ERR_INSERTION_ERROR;
    }
    
    node_t* new_node = current->right; //== NULL
    new_node = (node_t*) calloc(1, sizeof(node_t));
    if (!new_node) return TREE_ERR_ALLOCATION_ERROR;

    new_node->left = NULL;
    new_node->right = NULL;

    new_node->str = strdup(str);
    if (!new_node->str) return TREE_ERR_STR_PRINT_ERROR;

    current->right = new_node;

    return TREE_ERR_SUCCESS;
}
