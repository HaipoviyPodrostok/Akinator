#ifndef LIST_H
#define LIST_H

#include <cstdio>
#include <cstdlib>

#include "../utils/utils.h"

#define MAX_STR_SIZE 100
#define MAX_CONSOLE_STR_SIZE 50

//FILES
#define COUNTER_FILENAME "Text_dumps/counter.txt"
#define DOT_FILENAME "Text_dumps/tree_dump_%lu_%lu.dot"

//DOT FILE TEMPLATES
#define DOT_TITLE "digraph BinaryTree {\n    node [shape=\"Mrecord\", style=\"filled\", fontname=\"Courier New\"];\n"
#define NODE_OF_EMPTY_TREE "digraph EmptyTree {\n" \
    "   node [shape=box, style=rounded, fontname=\"Courier New\", color=gray];\n" \
    "   \"Empty\" [label=\"Tree is empty\"];}"

//SYSTEM COMMAND TEMPLATES
#define SYSTEM_COMMAND_TEMPLATE "dot Text_dumps/tree_dump_%lu_%lu.dot -T png -o Graphviz_dumps/tree_dump_%lu_%lu.png"

//COLORS
#define ROOT_COLOR "#FFFF00"
#define NODE_COLOR "#00FFFF"
#define LEFT_ARROW_COLOR "#008000"
#define RIGHT_ARROW_COLOR "#B22222"

typedef struct Node {
    char* str;   
    struct Node* left;
    struct Node* right;
} node_t;

typedef struct Tree {
    node_t* root;
    size_t size;
} tree_t;

typedef struct CallCounter {
    size_t call_num;
    size_t launch_num;
} call_cnt_t;

typedef enum NodeType {
    NODE_TYPE_ANSWER = 1,
    NODE_TYPE_QUESTION =2,
    NODE_TYPE_LEFT = 3,
    NODE_TYPE_RIGHT = 4,
    NODE_TYPE_ROOT = 5,
    NODE_TYPE_EMPTY_NODE = 6,
} node_type_t;

//CTOR
tree_err_t tree_ctor(tree_t* tree, call_cnt_t* call_cnt);
tree_err_t fill_tree_from_file(const char* const tree_source_filename, tree_t* tree, call_cnt_t* call_cnt);
tree_err_t fill_tree_recursion(tree_t* tree, FILE* tree_source, node_t* current, node_type_t node_type, call_cnt_t* call_cnt);
tree_err_t fill_node(FILE* tree_source, node_t** current, node_type_t node_type);

//ADD NOTE
tree_err_t insert_root(node_t** root, char* str);
tree_err_t insert_left(node_t* current, char* str);
tree_err_t insert_right(node_t* current, char* str);
tree_err_t user_inpurt(tree_t* tree);
tree_err_t input_recursion(node_t* current);
tree_err_t node_add(tree_t* tree, const char* str);
tree_err_t node_add_recursion(node_t* current, const char* str);

//DUMP
tree_err_t tree_dump(tree_t* tree, call_cnt_t* call_cnt);
tree_err_t dump_recursion(node_t* current, FILE* dot_file, tree_t* tree);
tree_err_t print_node(node_t* current, FILE* dot_file, tree_t* tree);
tree_err_t make_dot_filename(char** dot_filename, call_cnt_t* call_cnt);

//SAVE TREE
tree_err_t save_tree(tree_t* tree, const char* const filename);
tree_err_t save_tree_recursion(node_t* current, FILE* file_save_to, node_type_t node_type, int height);
void print_spaces(FILE* file_save_to, int height);

//DTOR
void tree_dtor(tree_t* tree);
void dtor_recursion(node_t* current);
void dtor_if(int err_num, int dtor_err_num, tree_t* tree);

//INTERFACE
tree_err_t handle_question(node_t* current);
tree_err_t handle_answer(node_t* current);
tree_err_t read_from_console(char* new_object);

#endif //LIST_H