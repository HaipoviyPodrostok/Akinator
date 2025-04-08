#include "stdio.h"
#include "stdlib.h"

#include "tree.h"

void dtor_if(int err_num, int dtor_err_num, tree_t* tree) {
    if (err_num == dtor_err_num) {
        tree_dtor(tree);
    }
}