#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>

#include "tree.h"

tree_err_t user_inpurt(tree_t* tree, call_cnt_t* call_cnt) {
    
    assert(tree != NULL);
    
    while (tree) {
        int action = 0;
        printf("==============================================\n"
               COLOR_RED"Меню:\n"COLOR_RESET COLOR_YELLOW
               "[1]"COLOR_RESET" играть\n" COLOR_YELLOW
               "[2]"COLOR_RESET" описание объекта\n" 
               "[3] сохранение дерева в файл\n"
               "[4] дамп дерева\n"
               "[5] выход\n");
        
        if (scanf("%d", &action) != 1) {
            perror("Can not");
            return TREE_ERR_INPUT_SCAN_ERROR;
        }

        switch(action) {
            case INPUT_ACTION_PLAY:
                ERROR_HANDLE(input_recursion(tree->root));
                break;

            case INPUT_ACTION_OBJECT_DESCRIPTION: {
                printf("Напишите имя объекта чтобы получить описание\n");
                
                char* object_name = NULL;
                scanf("%99ms", &object_name);
                
                ERROR_HANDLE(make_description(tree, object_name));
                break;
            }

            case INPUT_ACTION_SAVE_TREE_TO_FILE: {
                printf("Куда вы хотите сохранить файл?\n");              

                char* file_save_to_name = NULL;
                scanf("%99ms", &file_save_to_name);

                ERROR_HANDLE(save_tree(tree, file_save_to_name));
                break;
            }

            case INPUTACTION_TREE_DUMP:
                ERROR_HANDLE(tree_dump(tree, call_cnt));
                break;

            case INPUT_ACTION_EXIT:
                return TREE_ERR_SUCCESS;

            default:
                printf("Попробуте заново\n");
                break;
        }
    }
    return TREE_ERR_UNKNOWN_ERROR;
}

tree_err_t input_recursion(node_t* current) {
    
    int node_type = 0;
    
    if ((current->left != NULL) && (current->right != NULL)) node_type = NODE_TYPE_QUESTION;
    else if (!(current->left || current->right))             node_type = NODE_TYPE_ANSWER;
    else                                                     return TREE_ERR_INCORRECT_TREE;
    
    switch(node_type) {
        
        case NODE_TYPE_QUESTION:
            ERROR_HANDLE(handle_question(current));
        break;
        
        case NODE_TYPE_ANSWER:
            ERROR_HANDLE(handle_answer(current));
        break;
        
        default:
            return TREE_ERR_INCORRECT_TREE;
    }
    
    return TREE_ERR_SUCCESS;
}

tree_err_t handle_question(node_t* current) {
    
    assert(current != NULL);

    printf("Это %s? (\"да\" or \"нет\")\n", current->str);
            
    while (true) {
        char* answer = NULL;
        scanf("%10ms", &answer);
        
        if (strcasecmp(answer, "да") == 0) {
            free(answer);
            ERROR_HANDLE(input_recursion(current->left));
            break;
        }
        else if (strcasecmp(answer, "нет") == 0) {
            free(answer);
            ERROR_HANDLE(input_recursion(current->right));
            break;
        } 
        else {
            printf("Вы можете ответить только \"да\" или \"нет\", Попробуте снова\n");
            free(answer);
        }
    }

    return TREE_ERR_SUCCESS;
}

tree_err_t handle_answer(node_t* current) {
    
    assert(current != NULL);

    printf("Это %s. Я угадал?\n", current->str);
        
    while (true) {
        char* answer = NULL;
        scanf("%10ms", &answer);
        
        if (strcmp(answer, "да") == 0) {
            str_dtor(answer);
            printf("ГОЙДА\n");
            return TREE_ERR_SUCCESS;
        }
        else if (strcmp(answer, "нет") == 0) {
            str_dtor(answer);
            
            free_bufer();
            printf("Что тогда это было?\n");
            char new_object[MAX_CONSOLE_STR_SIZE] = {}; 
            ERROR_HANDLE(read_from_console(new_object));

            printf("Какими свойствами он обладает?\n");  
            char features[MAX_CONSOLE_STR_SIZE] = {};
            ERROR_HANDLE(read_from_console(features));
            
            insert_right(current, current->str);    
            
            str_dtor(current->str);
            current->str = strdup(features);
            if (!current->str) return TREE_ERR_ALLOCATION_ERROR;
            
            insert_left(current, new_object);
            
            printf("Новый объект записан\n");
            
            return TREE_ERR_SUCCESS;
        } 
        else {
            str_dtor(answer);
            printf("Вы можете ответить только \"да\" или \"нет\", Попробуте снова\n");
        }
    }

    return TREE_ERR_SUCCESS;
}

tree_err_t read_from_console(char* const str) {
    assert(str);
    
    if (!fgets(str, MAX_CONSOLE_STR_SIZE, stdin)) {
        perror("Can not fgets");
        return TREE_ERR_INPUT_SCAN_ERROR;
    }
    size_t str_len = strlen(str);
    
    if (str_len > 0 && (str)[str_len - 1] == '\n') {
        str[str_len - 1] = '\0';
    } else {
        printf("Название объекта слишком длинное, оно было сокращено до первых 50 символов\n");
        free_bufer();
    }
    return TREE_ERR_SUCCESS;
}

tree_err_t make_description(tree_t* tree, const char* object) {
    assert(tree);
    assert(object);

    if (tree->root) {
        if(strcasecmp(object, tree->root->str))
    }
    
    else  {
        printf("Дерево пустое\n");
        return TREE_ERR_SUCCESS;
    }


}

tree_err_t make_description_recursion(node_t* current, const char* object) {

}
