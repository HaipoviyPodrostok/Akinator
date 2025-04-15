#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cassert>

#include "stack.h"

int stack_ctor(stack_t* stk, int capacity) {
    
    stk->data = (stack_elem_t*) calloc((size_t) capacity + ALLOCATION_RESERVE, sizeof(stack_elem_t));   
        
    stk->size = 0;
    stk->capacity = capacity + 10;
    
    return 0;
}

int stack_dtor(stack_t* stk) {
    
    free(stk->data);
    
    stk->size = -1;
    stk->capacity = -1;
    stk->data = NULL;
    
    return 0;
}

void stack_capacity_check(stack_t* stk) {
                               
    size_t data_size = 0;

    data_size = (size_t) (stk->capacity) * sizeof(stack_elem_t);

    if (stk->size >= stk->capacity) {
        
        data_size += 10 * sizeof(stack_elem_t);
        stk->data = (stack_elem_t*) realloc(stk->data - sizeof(stack_elem_t*), data_size);  
        stk->capacity += 10; 
        stk->data[stk->capacity] = stk->data[stk->capacity - 10]; // 
        stk->data[stk->capacity - 10] = {NULL, 0};
    }

    else if (stk->capacity - stk->size == 20) {
        
        stk->data[stk->capacity - 10] = stk->data[stk->capacity];
        stk->data[stk->capacity] = {NULL, 0};
        
        stk->data = (stack_elem_t*) realloc(stk->data - sizeof(stack_elem_t*), data_size);  
        stk->capacity -= 10;      
    }
    
}

bool isEqual (double x, double y) {
    const double EPSILON = 1e-6;
    return fabs (x - y) < EPSILON;
}


