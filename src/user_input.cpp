#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>

#include "tree.h"

tree_err_t user_inpurt(tree_t* tree) {
    
    assert(tree != NULL);
    
    ERROR_HANDLE(input_recursion(tree->root));
    
    return TREE_ERR_SUCCESS;
}

tree_err_t input_recursion(node_t* current) {
    
    int node_type = 0;
    
    if ((current->left != NULL) && (current->right != NULL)) node_type = NODE_TYPE_QUESTION;
    else if (!(current->left || current->right))             node_type = NODE_TYPE_ANSWER;
    else                                                     return TREE_ERR_INCORRECT_TREE;
    
    switch(node_type) {
        
        case NODE_TYPE_QUESTION:
            ERROR_HANDLE(handle_question(current));
        break;
        
        case NODE_TYPE_ANSWER:
            ERROR_HANDLE(handle_answer(current));
        break;
        
        default:
            return TREE_ERR_INCORRECT_TREE;
    }
    
    return TREE_ERR_SUCCESS;
}

tree_err_t handle_question(node_t* current) {
    
    assert(current != NULL);

    printf("Is it %s? (\"yes\" or \"no\")\n", current->str);
            
    while (true) {
        char* answer = NULL;
        scanf("%ms", &answer);
        
        if (strcasecmp(answer, "yes") == 0) {
            free(answer);
            ERROR_HANDLE(input_recursion(current->left));
            break;
        }
        else if (strcasecmp(answer, "no") == 0) {
            free(answer);
            ERROR_HANDLE(input_recursion(current->right));
            break;
        } 
        else {
            printf("You can answer \"yes\" or \"no\", try again\n");
            free(answer);
        }
    }

    return TREE_ERR_SUCCESS;
}

tree_err_t handle_answer(node_t* current) {
    
    assert(current != NULL);

    printf("it is %s. Am I right?\n", current->str);
        
    while (true) {
        char* answer = NULL;
        scanf("%ms", &answer);
        
        if (strcmp(answer, "yes") == 0) {
            free(answer);
            printf("GOYDA\n");
            return TREE_ERR_SUCCESS;
        }
        else if (strcmp(answer, "no") == 0) {
            free(answer);
            
            printf("What is it then?\n");
            
            char* new_object = NULL; 
            scanf("%ms", &new_object);
            
            printf("what features does it have?\n");
            
            char* features = NULL;
            scanf("%ms", &features);
            
            insert_right(current, current->str);    
            
            free(current->str);
            current->str = NULL;
            current->str = strdup(features);
            if (!current->str) return TREE_ERR_ALLOCATION_ERROR;
            free(features);
            
            insert_left(current, new_object);
            free(new_object);
            
            printf("New object nad written\n");
            
            return TREE_ERR_SUCCESS;
        } 
        else {
            printf("You can answer \"yes\" or \"no\", try again\n");
            free(answer);
        }
    }

    return TREE_ERR_SUCCESS;
}