#ifndef UTILS_H
#define UTILS_H

#include <cstdio>
#include <cstdlib>

#define ERROR_HANDLE(call_func, ...) ({                                             \
    tree_err_t error = call_func;                                                   \
    if (error) {                                                                    \
        fprintf(stderr, "[" #call_func "] Error: %s\n", tree_error_str(error));     \
        __VA_ARGS__;                                                                \
        return error;                                                               \
    }                                                                               \
});

typedef enum { 
    TREE_ERR_SUCCESS = 0,
    TREE_ERR_DUMP_ERROR = 1,
    TREE_ERR_TREE_DOES_NOT_EXIST = 2,
    TREE_ERR_TREE_IS_EMPTY = 3,
    TREE_ERR_ALLOCATION_ERROR = 4,
    TREE_ERR_FILE_READING_ERROR = 5,
    TREE_ERR_INCORRECT_TREE = 6,
    TREE_ERR_FILE_OPEN_ERROR = 7,
    TREE_ERR_FILE_PRINT_ERROR = 8,
    TREE_ERR_NULL_PTR_ERROR = 9,
    TREE_ERR_FILE_CLOSE_ERROR = 10,
    TREE_ERR_STR_PRINT_ERROR = 11,
    TREE_ERR_INSERTION_ERROR = 12,
    TREE_ERR_INPUT_FILE_INCORRECT = 13,
    TREE_ERR_FILL_NODE_ERROR = 14,
    TREE_ERR_INPUT_SCAN_ERROR = 15,
    TREE_ERR_TOO_FEW_COMMAND_LINE_ARGUMENTS = 16,
    TREE_ERR_UNKNOWN_ERROR = 17,
} tree_err_t;

void free_bufer(void);
void skip_spaces(FILE* tree_source);
void str_dtor(char* str);
const char* tree_error_str(tree_err_t error);

#endif //UTILS_H  