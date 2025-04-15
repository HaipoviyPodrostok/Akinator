#include <cstdio>
#include <cstdlib>
#include <cassert>

#include "tree.h"

tree_err_t save_tree(tree_t* tree, const char* const filename) {
    assert(tree);
    assert(filename);

    FILE* file_save_to = fopen(filename, "w");
    if (!file_save_to) {
        perror("Can not open file");
        return TREE_ERR_FILE_OPEN_ERROR;
    }

    if (tree->root) {
        int height = 0;
        fprintf(file_save_to, "{\n    %s\n", tree->root->str);
        if (tree->root->left) {
            ERROR_HANDLE(save_tree_recursion(tree->root->left, file_save_to, NODE_TYPE_LEFT, height + 1));
        } 
        if (tree->root->right) {
            ERROR_HANDLE(save_tree_recursion(tree->root->right, file_save_to, NODE_TYPE_RIGHT, height + 1));
        }
        fprintf(file_save_to, "}");
        printf("Дерево успешно сохранено\n");
        
        return TREE_ERR_SUCCESS;
    } else {
        printf("Варнинг! Было записано пустое дерево\n");
        if (fclose(file_save_to) != 0) {
            perror("Can not fclose");
            return TREE_ERR_FILE_CLOSE_ERROR;
        }
    }
    return TREE_ERR_UNKNOWN_ERROR;
}

tree_err_t save_tree_recursion(node_t* current, FILE* file_save_to, node_type_t node_type, int height) {
    assert(current);
    assert(file_save_to);
    
    print_spaces(file_save_to, height);
    if (node_type == NODE_TYPE_LEFT)       fprintf(file_save_to,  "left {\n");
    else if (node_type == NODE_TYPE_RIGHT) fprintf(file_save_to, "right {\n");
    
    print_spaces(file_save_to, height + 1);
    fprintf(file_save_to, "%s\n", current->str);

    if (current->left) {
        ERROR_HANDLE(save_tree_recursion(current->left, file_save_to, NODE_TYPE_LEFT, height + 1));
    }
    if (current->right) {
        ERROR_HANDLE(save_tree_recursion(current->right, file_save_to, NODE_TYPE_RIGHT, height + 1));
    }
    print_spaces(file_save_to, height);
    fprintf(file_save_to, "}\n");
    
    return TREE_ERR_SUCCESS;
}


void print_spaces(FILE* file_save_to, int height) {
    assert(file_save_to);
    for (int i = 0; i < height; i++) {
        fprintf(file_save_to, "    ");
    }
}