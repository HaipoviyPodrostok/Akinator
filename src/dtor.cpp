#include <cstdio>
#include <cstdlib>
#include <cassert>

#include "tree.h"

void tree_dtor(tree_t* tree) {
    
    assert (tree != NULL);

    dtor_recursion(tree->root);
}

void dtor_recursion(node_t* current) {
    
    if (current) {
        if (current->left)  dtor_recursion(current->left);
        if (current->right) dtor_recursion(current->right); 
        
        current->left = NULL;
        current->right = NULL;
        
        free(current->str);
        current->str = NULL;
        free(current);
    }
}
