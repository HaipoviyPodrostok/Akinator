#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "tree.h"

int main(int argc, char* argv[]) {
    
    tree_t tree = {};
    call_cnt_t call_cnt = {};

    if (argc != 3) {
        fprintf(stderr, "You need 3 arguments to run the program.\n");
        return TREE_ERR_TOO_FEW_COMMAND_LINE_ARGUMENTS;
    }
    
    const char* const tree_source_filename = argv[1];
    const char* const file_save_to_name    = argv[2]; 
    
    ERROR_HANDLE(tree_ctor(&tree, &call_cnt), tree_dtor(&tree));
    
    ERROR_HANDLE(fill_tree_from_file(tree_source_filename, &tree, &call_cnt), tree_dtor(&tree));

    ERROR_HANDLE(user_inpurt(&tree), tree_dtor(&tree));

    ERROR_HANDLE(tree_dump(&tree, &call_cnt), tree_dtor(&tree));
    
    ERROR_HANDLE(save_tree(&tree, file_save_to_name), tree_dtor(&tree));
    
    tree_dtor(&tree);

    return 0;
}