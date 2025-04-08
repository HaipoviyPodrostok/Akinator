#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "tree.h"

int main(int argc, char* argv[]) {
    
    tree_t tree = {};
    call_cnt_t call_cnt = {};

    char* tree_source_filename = argv[1];
    
    ERROR_HANDLE(tree_ctor(&tree, &call_cnt), tree_dtor(&tree));
    
    //ERROR_HANDLE(fill_tree_from_file(tree_source_filename, &tree));

    char str[100] = "";
    strcpy(str, "hyi");
    ERROR_HANDLE(insert_root(&tree, str));
    printf("%s\n", tree.root->str);
    //ERROR_HANDLE(user_inpurt(&tree), tree_dtor(&tree));
    
    ERROR_HANDLE(tree_dump(&tree, &call_cnt));
    ERROR_HANDLE(tree_dump(&tree, &call_cnt));
 
    tree_dtor(&tree);

    return 0;
}