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
    system("mkdir -p Text_dumps");
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
        fprintf(stderr, "Корень не пустой!\n");
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

tree_err_t make_description(tree_t* tree, const char* object, stack_t* stack) {
    assert(tree);
    assert(object);

    if (tree->root) {
        if (tree->root->left) {
            stack_push(stack, {tree->root->str, NODE_TYPE_LEFT});
            
            bool isFound = false;
            ERROR_HANDLE(make_description_recursion(tree->root->left, object, stack, &isFound));
            
            if      (!isFound) stack_pop(stack);
            else if (isFound)  return TREE_ERR_SUCCESS;
        }
        if (tree->root->right) {
            stack_push(stack, {tree->root->str, NODE_TYPE_RIGHT});
            
            bool isFound = false;
            ERROR_HANDLE(make_description_recursion(tree->root->right, object, stack, &isFound));
            
            if      (!isFound) stack_pop(stack);
            else if (isFound)  return TREE_ERR_SUCCESS;
        }
        
        slow_print(BOLD CYAN "Объект не найден\n" RESET, SLOW_PRINT_DELAY);
        return TREE_ERR_SUCCESS;
    } 
    else  {
        slow_print(BOLD RED "\nОшибка: видимо вы забыли загрузить дерево, либо загруженное вами дерево пусто\n" RESET, SLOW_PRINT_DELAY);
        return TREE_ERR_SUCCESS;
    }
}

tree_err_t make_description_recursion(node_t* current, const char* object, stack_t* stack, bool* isFound_ptr) {
    assert(current);
    assert(object);

    if (current->left) {
        stack_push(stack, {current->str, NODE_TYPE_LEFT});
        
        bool isFound = false;
        ERROR_HANDLE(make_description_recursion(current->left, object, stack, &isFound));
        
        if (!isFound) {
            stack_pop(stack);
        } 
        else if (isFound) {
            *isFound_ptr = true;
            return TREE_ERR_SUCCESS;
        }
        else return TREE_ERR_UNKNOWN_ERROR;
    }
    if (current->right) {
        stack_push(stack, {current->str, NODE_TYPE_RIGHT});
        
        bool isFound = false;
        ERROR_HANDLE(make_description_recursion(current->right, object, stack, &isFound));
        
        if (!isFound) {
            stack_pop(stack);
        } 
        else if (isFound) {
            *isFound_ptr = true;
            return TREE_ERR_SUCCESS;
        }
        else return TREE_ERR_UNKNOWN_ERROR;
    }
    else if (strcasecmp(current->str, object) == 0) {
        *isFound_ptr = true;
    }
    return TREE_ERR_SUCCESS;
}

tree_err_t print_stack(stack_t* stack) {
    assert(stack);
     
    while(stack->size > 0) {
        stack_elem_t stack_peak = {NULL, 0};
        stack_peak = stack_pop(stack);
        if      (stack_peak.node_type == NODE_TYPE_LEFT)  printf(YELLOW   " - %s\n"    RESET, stack_peak.str);
        else if (stack_peak.node_type == NODE_TYPE_RIGHT) printf(BOLD RED " - не " RESET YELLOW "%s\n" RESET, stack_peak.str);
        else    return TREE_ERR_UNKNOWN_ERROR;
    }
    
    return TREE_ERR_SUCCESS;
}

void stack_reverse(stack_t* stack, stack_t* inverted_stack) {
    assert(stack);
    assert(inverted_stack);

    while (stack->size > 0) {
        stack_push(inverted_stack, stack_pop(stack));
    }
}

tree_err_t make_comparison(tree_t* tree, const char* first_object, const char* second_object) {
    stack_t obj_desc1 = {};
    stack_ctor(&obj_desc1, STACK_CAPACITY);
    
    ERROR_HANDLE(make_description(tree, first_object, &obj_desc1));

    stack_t obj_desc_inv1 = {};
    stack_ctor(&obj_desc_inv1, STACK_CAPACITY);
    
    stack_reverse(&obj_desc1, &obj_desc_inv1);

    stack_t obj_desc2 = {};
    stack_ctor(&obj_desc2, STACK_CAPACITY);
    
    ERROR_HANDLE(make_description(tree, second_object, &obj_desc2));

    stack_t obj_desc_inv2 = {};
    stack_ctor(&obj_desc_inv2, STACK_CAPACITY);
    
    stack_reverse(&obj_desc2, &obj_desc_inv2);

    printf(BOLD CYAN "Общие признаки:\n" RESET);

    stack_elem_t stack_peak1 = {};
    stack_elem_t stack_peak2 = {};

    while (obj_desc_inv1.size != 0 || obj_desc_inv2.size != 0) {
        stack_peak1 = stack_pop(&obj_desc_inv1); 
        stack_peak2 = stack_pop(&obj_desc_inv2);

        if ((strcasecmp(stack_peak1.str, stack_peak2.str) == 0) && (stack_peak1.node_type == stack_peak2.node_type)) {
            if      (stack_peak1.node_type == NODE_TYPE_LEFT)  printf(YELLOW" - %s\n" RESET, stack_peak1.str);
            else if (stack_peak1.node_type == NODE_TYPE_RIGHT) printf(BOLD RED " - не " RESET YELLOW "%s\n" RESET, stack_peak1.str);
            else    return TREE_ERR_UNKNOWN_ERROR;
        } else break;
    }

    if (obj_desc_inv1.size != 0 || obj_desc_inv2.size != 0) {
        stack_push(&obj_desc_inv1, stack_peak1);
        stack_push(&obj_desc_inv2, stack_peak2);
        
        printf(BOLD CYAN "Отличительные признаки первого объекта:\n" RESET);
        print_stack(&obj_desc_inv1);
        printf(BOLD CYAN "Отличительные признаки второго объекта:\n" RESET);
        print_stack(&obj_desc_inv2);
    }

    stack_dtor(&obj_desc1);
    stack_dtor(&obj_desc2);
    stack_dtor(&obj_desc_inv1);
    stack_dtor(&obj_desc_inv2);

    return TREE_ERR_SUCCESS;
}

tree_err_t isObjExist(tree_t* tree, const char* object, bool* isExistPtr) {
    if (tree->root) {
        if (tree->root->left) {          
            bool isFound = false;
            ERROR_HANDLE(isObjExistRec(tree->root->left, object, &isFound));
            
            if (isFound)  {
                *isExistPtr = true;
                return TREE_ERR_SUCCESS;
            }
        }
        if (tree->root->right) {            
            bool isFound = false;
            ERROR_HANDLE(isObjExistRec(tree->root->right, object, &isFound));
            
            if (isFound)  {
                *isExistPtr = true;
                return TREE_ERR_SUCCESS;
            }
        }
        
        slow_print(BOLD CYAN "Объект не найден\n" RESET, SLOW_PRINT_DELAY);
        return TREE_ERR_SUCCESS;
    } 
    else  {
        slow_print(BOLD RED "\nОшибка: видимо вы забыли загрузить дерево, либо загруженное вами дерево пусто\n" RESET, SLOW_PRINT_DELAY);
        return TREE_ERR_SUCCESS;
    }
}

tree_err_t isObjExistRec(node_t* current, const char* object, bool* isExistPtr) {
    assert(current);
    assert(object);

    if (current->left) {        
        bool isExist = false;
        ERROR_HANDLE(isObjExistRec(current->left, object, &isExist));
        
        if (isExist) {
            *isExistPtr = true;
            return TREE_ERR_SUCCESS;
        }
    }
    if (current->right) {        
        bool isExist = false;
        ERROR_HANDLE(isObjExistRec(current->right, object, &isExist));
        
        if (isExist) {
            *isExistPtr = true;
            return TREE_ERR_SUCCESS;
        }
    }
    else if (strcasecmp(current->str, object) == 0) {
        *isExistPtr = true;
    }
    return TREE_ERR_SUCCESS;
}
