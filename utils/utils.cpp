#include <cstdio>
#include <cstdlib>
#include <cassert>

#include "utils.h"

#define CASE_ENUM_TO_STRING(error) case error: return #error
const char* tree_error_str(tree_err_t error) {
    switch (error) {
        CASE_ENUM_TO_STRING(TREE_ERR_SUCCESS);
        CASE_ENUM_TO_STRING(TREE_ERR_DUMP_ERROR);
        CASE_ENUM_TO_STRING(TREE_ERR_TREE_DOES_NOT_EXIST);
        CASE_ENUM_TO_STRING(TREE_ERR_TREE_IS_EMPTY);
        CASE_ENUM_TO_STRING(TREE_ERR_ALLOCATION_ERROR);
        CASE_ENUM_TO_STRING(TREE_ERR_FILE_READING_ERROR);
        CASE_ENUM_TO_STRING(TREE_ERR_INCORRECT_TREE);
        CASE_ENUM_TO_STRING(TREE_ERR_FILE_OPEN_ERROR);
        CASE_ENUM_TO_STRING(TREE_ERR_FILE_PRINT_ERROR);
        CASE_ENUM_TO_STRING(TREE_ERR_NULL_PTR_ERROR);
        CASE_ENUM_TO_STRING(TREE_ERR_FILE_CLOSE_ERROR);
        CASE_ENUM_TO_STRING(TREE_ERR_STR_PRINT_ERROR);
        CASE_ENUM_TO_STRING(TREE_ERR_INSERTION_ERROR);
        CASE_ENUM_TO_STRING(TREE_ERR_INPUT_FILE_INCORRECT);
        CASE_ENUM_TO_STRING(TREE_ERR_FILL_NODE_ERROR);
        CASE_ENUM_TO_STRING(TREE_ERR_INPUT_SCAN_ERROR);
        CASE_ENUM_TO_STRING(TREE_ERR_TOO_FEW_COMMAND_LINE_ARGUMENTS);
        CASE_ENUM_TO_STRING(TREE_ERR_UNKNOWN_ERROR);
        CASE_ENUM_TO_STRING(TREE_ERR_INCORRECT_STATUS);
        default:
            return "UNKNOWN_TREE_ERROR";
        return "UNKNOWN_TREE_ERROR";
    }
}
#undef CASE_ENUM_TO_STRING

void free_bufer(void) {
    int ch = 0;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

void skip_spaces(FILE* tree_source){
    assert(tree_source);
    fscanf(tree_source, "%*[ \t\n]");
}

void str_dtor(char* str) {
    free(str);
    str = NULL;
}