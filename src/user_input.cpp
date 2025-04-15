#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <unistd.h>

#include "tree.h"

tree_err_t user_inpurt(tree_t* tree, call_cnt_t* call_cnt) {
    
    assert(tree != NULL);
    
    while (tree) {
        int action = 0;

        printf("\n\n");
        printf(BOLD PURPLE "*********************************************\n" RESET);
        printf(BOLD PURPLE "*                  " BLINK "АКИНАТОР" PURPLE "              *\n" RESET);
        printf(BOLD PURPLE "*********************************************\n\n" RESET);
        printf(BOLD YELLOW "1. " RESET GREEN "Начать новую игру\n" RESET);
        printf(BOLD YELLOW "2. " RESET GREEN "Загрузить сохранение из файла\n" RESET);
        printf(BOLD YELLOW "3. " RESET GREEN "Составить описание объекта\n" RESET);
        printf(BOLD YELLOW "4. " RESET GREEN "Сохранить дерево в файл\n" RESET);
        printf(BOLD YELLOW "5. " RESET GREEN "Сравнить два объекта\n" RESET);   
        printf(BOLD YELLOW "6. " RESET GREEN "Графический дамп дерева\n" RESET);
        printf(BOLD YELLOW "7. " RESET GREEN "Об игре\n" RESET);
        printf(BOLD YELLOW "8. " RESET GREEN "Выход\n\n" RESET);
        printf(BOLD CYAN "Выберите пункт меню: " YELLOW);
            
        if (scanf("%d", &action) != 1) {
            perror("Can not");
            return TREE_ERR_INPUT_SCAN_ERROR;
        }

        switch(action) {
            
            case INPUT_ACTION_PLAY: 
                if (tree->root == NULL) {
                    slow_print(BOLD RED "\nОшибка: видимо вы забыли загрузить дерево, либо загруженное вами дерево пусто\n" RESET, 5000);
                    sleep(2);
                } else {
                    ERROR_HANDLE(input_recursion(tree->root));
                }
                break;

            case INPUT_ACTION_IMPORT_TREE: {
                printf(BOLD CYAN "\nИз какого файла вы хотите заполнить дерево?:\n" PURPLE);
                
                char* filename_str = NULL;
                scanf("%99ms", &filename_str);
                
                FILE* tree_source = fopen(filename_str, "r");
                if (!tree_source) {
                    slow_print(BOLD RED "Ошибка: ")
                    return TREE_ERR_FILE_OPEN_ERROR;
                }
            
                ERROR_HANDLE(fill_tree_from_file(tree_source, tree, call_cnt), tree_dtor(tree));
                
                str_dtor(filename_str); 
                break;
            }
            
            case INPUT_ACTION_OBJECT_DESCRIPTION: {
                printf(BOLD CYAN "\nНапишите имя объекта, чтобы получить описание\n" RESET);
                
                char* object_name = NULL;
                scanf("%99ms", &object_name);
                
                ERROR_HANDLE(make_description(tree, object_name));

                str_dtor(object_name);
                break;
            }

            case INPUT_ACTION_SAVE_TREE_TO_FILE: {
                printf(BOLD CYAN "\nКуда вы хотите сохранить файл?\n" RESET);              

                char* file_save_to_name = NULL;
                scanf("%99ms", &file_save_to_name);

                ERROR_HANDLE(save_tree(tree, file_save_to_name));

                str_dtor(file_save_to_name);
                break;
            }

            case INPUT_ACTION_OBJECTS_COMPARISON:
                break;
            
            case INPUT_ACTION_ABOUT_GAME:
                printf(BOLD RED "Акинатор" BOLD CYAN " - это игра, в которой компьютер пытается угадать задуманный вами персонаж или предмет с помощью наводящих вопросов.\n" RESET);
                break;

            case INPUT_ACTION_TREE_DUMP:
                ERROR_HANDLE(tree_dump(tree, call_cnt));
                break;

            case INPUT_ACTION_EXIT:
                return TREE_ERR_SUCCESS;

            default:
                printf(BOLD CYAN "Попробуте заново\n" RESET);
                break;
        }
    }
    return TREE_ERR_UNKNOWN_ERROR;
}

tree_err_t input_recursion(node_t* current) {
    
    assert(current);
        
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

    stack_t stack = {};
    stack_ctor(&stack, STACK_CAPACITY);

    if (tree->root) {
        if (tree->root->left) {
            stack_push(&stack, {tree->root->str, NODE_TYPE_LEFT});
            
            if_found_t status = IF_FOUND_NOT_FOUND;
            make_description_recursion(tree->root->left, object, &stack, &status);
            
            if      (status == IF_FOUND_NOT_FOUND) stack_pop(&stack);
            else if (status == IF_FOUND_FOUND)     return TREE_ERR_SUCCESS;
        }
        if (tree->root->right) {
            stack_push(&stack, {tree->root->str, NODE_TYPE_RIGHT});
            
            if_found_t status = IF_FOUND_NOT_FOUND;
            make_description_recursion(tree->root->right, object, &stack, &status);
            
            if      (status == IF_FOUND_NOT_FOUND) stack_pop(&stack);
            else if (status == IF_FOUND_FOUND)     return TREE_ERR_SUCCESS;
        }
        
        printf("Объект не найден\n");
        return TREE_ERR_SUCCESS;
    } 
    else  {
        printf("Дерево пустое\n");
        return TREE_ERR_SUCCESS;
    }
}

tree_err_t make_description_recursion(node_t* current, const char* object, stack_t* stack, if_found_t* status_ptr) {
    assert(current);
    assert(object);

    if (current->left) {
        stack_push(stack, {current->str, NODE_TYPE_LEFT});
        
        if_found_t status = IF_FOUND_NOT_FOUND;
        make_description_recursion(current->left, object, stack, &status);
        
        if (status == IF_FOUND_NOT_FOUND) {
            stack_pop(stack);
        } 
        else if (status == IF_FOUND_FOUND) {
            *status_ptr = IF_FOUND_FOUND;
            return TREE_ERR_SUCCESS;
        }
        else return TREE_ERR_UNKNOWN_ERROR;
    }
    if (current->right) {
        stack_push(stack, {current->str, NODE_TYPE_RIGHT});
        
        if_found_t status = IF_FOUND_NOT_FOUND;
        make_description_recursion(current->right, object, stack, &status);
        
        if (status == IF_FOUND_NOT_FOUND) {
            stack_pop(stack);
        } 
        else if (status == IF_FOUND_FOUND) {
            *status_ptr = IF_FOUND_FOUND;
            return TREE_ERR_SUCCESS;
        }
        else return TREE_ERR_UNKNOWN_ERROR;
    }
    else if (strcasecmp(current->str, object) == 0) {
        printf("Объект обладает следующими свойствами:\n");
        print_stack(stack);
        *status_ptr = IF_FOUND_FOUND;
    }
    else return TREE_ERR_SUCCESS;
    return TREE_ERR_UNKNOWN_ERROR;
}

tree_err_t print_stack(stack_t* stack) {
    assert(stack);
    
    stack_t inverted_stack = {};
    stack_ctor(&inverted_stack, STACK_CAPACITY);         //TODO статик переменные для стека

    stack_reverse(stack, &inverted_stack);
 
    while(inverted_stack.size > 0) {
        stack_elem_t stack_peak = {NULL, 0};
        stack_peak = stack_pop(&inverted_stack);
        if      (stack_peak.node_type == NODE_TYPE_LEFT)  printf("%s\n", stack_peak.str);
        else if (stack_peak.node_type == NODE_TYPE_RIGHT) printf("не %s\n", stack_peak.str);
        else    return TREE_ERR_UNKNOWN_ERROR;
    }
    
    return TREE_ERR_SUCCESS;
}

tree_err_t stack_reverse(stack_t* stack, stack_t* inverted_stack) {
    assert(stack);
    assert(inverted_stack);

    while (stack->size > 0) {      // TODO обработать ошибки стека
        stack_push(inverted_stack, stack_pop(stack));
    }

    return TREE_ERR_SUCCESS;
}

void slow_print(const char* str, int delay) {
    for (int i = 0; str[i]; i++) {
        putchar(str[i]);
        fflush(stdout);
        usleep(delay);
    }
}