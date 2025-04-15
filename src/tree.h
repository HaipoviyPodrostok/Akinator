#ifndef LIST_H
#define LIST_H

#include <cstdio>
#include <cstdlib>

#include "../utils/utils.h"
#include "../AkinatorStack/stack.h"

#define MAX_STR_SIZE 100
#define MAX_CONSOLE_STR_SIZE 50
#define STACK_CAPACITY 20

//FILES
#define COUNTER_FILENAME "Text_dumps/counter.txt"
#define DOT_FILENAME "Text_dumps/tree_dump_%lu_%lu.dot"
#define GRAPHIZ_DUMPS_FOLDER "Graphviz_dumps"

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

#define RESET "\x1B[0m"
#define BOLD "\x1B[1m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define PURPLE "\x1B[35m"
#define CYAN "\x1B[36m"
#define BLINK "\x1B[5m"

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
    NODE_TYPE_STACK_CANARY = 7,
} node_type_t;

typedef enum InputAction {
    INPUT_ACTION_PLAY = 1,
    INPUT_ACTION_IMPORT_TREE = 2,
    INPUT_ACTION_OBJECT_DESCRIPTION = 3,
    INPUT_ACTION_SAVE_TREE_TO_FILE = 4,
    INPUT_ACTION_OBJECTS_COMPARISON = 5,
    INPUT_ACTION_ABOUT_GAME = 6,
    INPUT_ACTION_TREE_DUMP = 7,
    INPUT_ACTION_EXIT = 8,
} input_action_t;

typedef enum ifFound {
    IF_FOUND_NOT_FOUND = 0,
    IF_FOUND_FOUND = 1,
} if_found_t;

//CTOR
tree_err_t tree_ctor(tree_t* tree, call_cnt_t* call_cnt);
tree_err_t fill_tree_from_file(FILE* tree_source, tree_t* tree, call_cnt_t* call_cnt);
tree_err_t fill_tree_recursion(tree_t* tree, FILE* tree_source, node_t* current, node_type_t node_type, call_cnt_t* call_cnt);
tree_err_t fill_node(FILE* tree_source, node_t** current, node_type_t node_type);

//ADD NOTE
tree_err_t insert_root(node_t** root, char* str);
tree_err_t insert_left(node_t* current, char* str);
tree_err_t insert_right(node_t* current, char* str);
tree_err_t user_inpurt(tree_t* tree, call_cnt_t* call_cnt);
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
tree_err_t read_from_console(char* const str);
tree_err_t make_description(tree_t* tree, const char* object);
tree_err_t make_description_recursion(node_t* current, const char* object, stack_t* stack, if_found_t* status_ptr);
tree_err_t print_stack(stack_t* stack);
tree_err_t stack_reverse(stack_t* stack, stack_t* inverted_stack);
void slow_print(const char* str, int delay);


#endif //LIST_H