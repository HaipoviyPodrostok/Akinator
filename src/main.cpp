#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "tree.h"

int main() {
    
    tree_t tree = {};
    call_cnt_t call_cnt = {};
        
    ERROR_HANDLE(tree_ctor(&tree, &call_cnt), tree_dtor(&tree));
    
    ERROR_HANDLE(user_inpurt(&tree, &call_cnt), tree_dtor(&tree));
        
    tree_dtor(&tree);

    return 0;
}