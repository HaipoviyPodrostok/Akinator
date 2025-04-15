#ifndef STACK_STRUCTS_H
#define STACK_STRUCTS_H

#define ALLOCATION_RESERVE 10

typedef struct SackElem {
    const char* str;
    int node_type;
} stack_elem_t;

typedef struct Stack {
    stack_elem_t* data;
    int size;
    int capacity;
} stack_t;

enum INPUT_ACTIONS {
    PUSH = 1,
    POP = 2,
    VERIFICATOR = 3,
    STACK_DUMP = 4,
    EXIT = 5,
};

enum ERRORS {
    ERROR_STACK_NULL_POINTER = 6,
    ERROR_DATA_NULL_POINTER = 7,
    ERROR_NEGATIVE_SIZE = 8,
    ERROR_NEGATIVE_CAPACITY = 9,
    ERROR_SIZE_OVERFLOW = 10,
    ERROR_STACK_CANARY_BEGINING = 11,
    ERROR_STACK_CANARY_END = 12,
    EMPTY_STACK_ERROR = 13,
};

//STACK CREATE
int stack_ctor(stack_t* stk, int capacity); 
int stack_dtor(stack_t* stk); 
void stack_capacity_check(stack_t* stk);

//FUNCS
int stack_push(stack_t* stk, stack_elem_t value);
stack_elem_t stack_pop(stack_t* stk);

//VERIFICATOR
int stack_check(const stack_t* stk);
void stack_dump(const stack_t* stk);
int verificator(const stack_t* stk);
void print_error(const stack_t* stk);

//UTILS
bool isEqual (double x, double y);

#endif //STACK_STRUCTS_H
