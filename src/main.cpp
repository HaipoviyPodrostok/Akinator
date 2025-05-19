#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "tree.h"

int main() {
    
    tree_t tree = {};
    call_cnt_t call_cnt = {};
        
    ERROR_HANDLE(tree_ctor(&tree, &call_cnt), tree_dtor(&tree));
    
    ERROR_HANDLE(user_inpurt(&tree, &call_cnt), tree_dtor(&tree));
    
    FILE* file_from = fopen("assets/baze.txt", "r");
    fill_tree_from_file(file_from, &tree, &call_cnt);

    FILE* file = fopen("assets/save.txt", "w");
    save_tree(&tree, file);

    tree_dtor(&tree);

    return 0;
}