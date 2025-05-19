#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <unistd.h>

#include "tree.h"

static void print_menu(void);
enum file_choice {
    FILE_CHOICE_DEFAULT_FILE = 1,
    FILE_CHOICE_USER_FILE = 2,
};

tree_err_t user_inpurt(tree_t* tree, call_cnt_t* call_cnt) {
    
    assert(tree != NULL);
    
    printf("\n\n");
    printf(BOLD PURPLE "*********************************************\n" RESET);
    slow_print(BOLD PURPLE "*                " BLINK "АКИНАТОР" PURPLE "                   *\n" RESET, SLOW_PRINT_DELAY);
    slow_print(BOLD PURPLE "*********************************************\n" RESET, SLOW_PRINT_DELAY);    
    
    bool ifUploaded = false;
    
    while (tree) { 
        print_menu();
        
        int action = 0;
        if (scanf("%d", &action) != 1) {
            action = INPUT_ACTION_TRY_AGAIN;
        }

        switch(action) {
            
            case INPUT_ACTION_PLAY: 
                if (tree->root == NULL) {
                    slow_print(BOLD RED "\nОшибка: видимо вы забыли загрузить дерево, либо загруженное вами дерево пусто\n" RESET, SLOW_PRINT_DELAY);
                    sleep(DEFAULT_SLEEP_TIME);
                } else {
                    ERROR_HANDLE(input_recursion(tree->root));
                }
                break;

            case INPUT_ACTION_IMPORT_TREE: {
                ERROR_HANDLE(import_tree_interface(tree, call_cnt, ifUploaded));
                break;
            }

            case INPUT_ACTION_OBJECT_DESCRIPTION: {
                ERROR_HANDLE(object_description_interface(tree));
                break;
            }

            case INPUT_ACTION_SAVE_TREE_TO_FILE: {
                ERROR_HANDLE(save_tree_interface(tree));
                break;
            }

            case INPUT_ACTION_OBJECTS_COMPARISON:
                ERROR_HANDLE(make_comparison_interface(tree));
                break;
            
            case INPUT_ACTION_ABOUT_GAME:
                slow_print(BOLD YELLOW "\nАкинатор" CYAN " - это игра, в которой компьютер пытается угадать задуманный вами персонаж или предмет с помощью наводящих вопросов.\n" RESET, SLOW_PRINT_DELAY);
                sleep(DEFAULT_SLEEP_TIME);
                break;

            case INPUT_ACTION_TREE_DUMP:
                ERROR_HANDLE(tree_dump(tree, call_cnt));
                break;

            case INPUT_ACTION_EXIT:
                return TREE_ERR_SUCCESS;

            case INPUT_ACTION_TRY_AGAIN:
            default:
                free_bufer();
                slow_print(BOLD CYAN "\nПопробуте заново\n" RESET, SLOW_PRINT_DELAY);
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
        scanf("%99ms", &answer);
        
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

static void print_menu(void) {
    slow_print(BOLD YELLOW "\n1. " RESET GREEN "Начать новую игру\n" RESET, SLOW_PRINT_DELAY);
    slow_print(BOLD YELLOW "2. " RESET GREEN "Загрузить сохранение из файла\n" RESET, SLOW_PRINT_DELAY);
    slow_print(BOLD YELLOW "3. " RESET GREEN "Составить описание объекта\n" RESET, SLOW_PRINT_DELAY);
    slow_print(BOLD YELLOW "4. " RESET GREEN "Сохранить дерево в файл\n" RESET, SLOW_PRINT_DELAY);
    slow_print(BOLD YELLOW "5. " RESET GREEN "Сравнить два объекта\n" RESET, SLOW_PRINT_DELAY);   
    slow_print(BOLD YELLOW "6. " RESET GREEN "Графический дамп дерева\n" RESET, SLOW_PRINT_DELAY);
    slow_print(BOLD YELLOW "7. " RESET GREEN "Об игре\n" RESET, SLOW_PRINT_DELAY);
    slow_print(BOLD YELLOW "8. " RESET GREEN "Выход\n\n" RESET, SLOW_PRINT_DELAY);
    slow_print(BOLD CYAN "Выберите пункт меню: " BOLD YELLOW, SLOW_PRINT_DELAY);
}

void slow_print(const char* str, unsigned int delay) {
    for (int i = 0; str[i]; i++) {
        putchar(str[i]);
        fflush(stdout);
        usleep(delay);
    }
}

tree_err_t import_tree_interface(tree_t* tree, call_cnt_t* call_cnt, bool ifUploaded) {
    if (ifUploaded) {
        printf(BOLD YELLOW "\nДерево уже загружено\n" RESET);
        return TREE_ERR_SUCCESS;
    }
    
    while (true) {
        printf(BOLD CYAN "\nВы можете загрузить свое дерево или использовать предустановленное:\n\n"
               BOLD YELLOW "    1. " RESET GREEN "Загрузить предустановленное дерево\n" RESET
               BOLD YELLOW "    2. " RESET GREEN "Загрузить дерево из файла\n" RESET);
        
        int file_choice = 0;
        scanf("%d", &file_choice);

        char path_to_file[100] = TREE_SOURCE_FILE;
        
        if (file_choice == FILE_CHOICE_DEFAULT_FILE) {
            FILE* tree_source = fopen(path_to_file, "r");
            if (tree_source) {
                ERROR_HANDLE(fill_tree_from_file(tree_source, tree, call_cnt));
                ifUploaded = true;
            }
            else {
                return TREE_ERR_FILE_OPEN_ERROR;
            }
            return TREE_ERR_SUCCESS;
        } 
        else if (file_choice == FILE_CHOICE_USER_FILE) {
            while (true) {
                printf(BOLD CYAN "\nНапишите относительный путь к файлу: " RESET YELLOW);
                if (scanf("%s", path_to_file) == 1) {
                    FILE* tree_source = fopen(path_to_file, "r");
                    if (tree_source) {
                        ERROR_HANDLE(fill_tree_from_file(tree_source, tree, call_cnt));
                        ifUploaded = true;
                        break;
                    } else {
                        slow_print(BOLD RED "\nОшибка: нет такого файла, попробуйте снова\n" RESET, SLOW_PRINT_DELAY);
                    }
                } else {
                    slow_print(BOLD CYAN "Попробуйте снова\n" RESET, SLOW_PRINT_DELAY);
                    sleep(DEFAULT_SLEEP_TIME);
                }
            }
            return TREE_ERR_SUCCESS;
        }
        else {
            printf(BOLD CYAN "Попробуйте еще раз\n" RESET);
            sleep(DEFAULT_SLEEP_TIME);
        }
    }
    return TREE_ERR_UNKNOWN_ERROR;
}

tree_err_t save_tree_interface(tree_t* tree) {
    printf(BOLD CYAN "\nНапишите относительный путь к файлу: " RESET);

    while (true) {
        char file_save_to_name [MAX_STR_SIZE] = {};
        scanf("%99s", file_save_to_name);

        FILE* file_save_to = fopen(file_save_to_name, "w");
        if (file_save_to) {
            save_tree(tree, file_save_to);
            break;
        } else {
            slow_print(BOLD RED "\nНет такого файла\n" RESET, SLOW_PRINT_DELAY);
        }
        sleep(DEFAULT_SLEEP_TIME);
    }
    return TREE_ERR_SUCCESS;
}

tree_err_t object_description_interface(tree_t* tree) {
    printf(BOLD CYAN "\nНапишите имя объекта, чтобы получить описание: " RESET);
                
    stack_t stack = {};
    stack_ctor(&stack, STACK_CAPACITY);

    char* object_name = NULL;
    scanf("%99ms", &object_name);
    
    ERROR_HANDLE(make_description(tree, object_name, &stack));

    stack_t stack_inv = {};
    stack_ctor(&stack_inv, STACK_CAPACITY);

    stack_reverse(&stack, &stack_inv);

    printf(BOLD PURPLE "\nОбъект обладает следующими свойствами:\n" RESET);
    print_stack(&stack_inv);        

    str_dtor(object_name);

    stack_dtor(&stack);
    stack_dtor(&stack_inv);

    return TREE_ERR_SUCCESS;
}

tree_err_t make_comparison_interface(tree_t* tree) {
    while (true) {
        printf(BOLD CYAN "\nНапишите названия объектов и я сравню их:\n" RESET);
        printf(BOLD CYAN "  Название первого объекта: " RESET);
    
        char first_object[MAX_CONSOLE_STR_SIZE] = {};
        if (scanf("%49s", first_object) != 1) {
            perror("Can not scanf");
            return TREE_ERR_INPUT_SCAN_ERROR;
        }
        
        bool isFirstExist = false;
        ERROR_HANDLE(isObjExist(tree, first_object, &isFirstExist));

        if (isFirstExist == false) {
            printf(BOLD CYAN "Попробуйте еще раз\n" RESET);
            continue;
        }

        printf(BOLD CYAN "  Название второго объекта: " RESET);

        char second_object[MAX_CONSOLE_STR_SIZE] = {};
        if (scanf("%49s", second_object) != 1) {
            perror("Can not scanf");
            return TREE_ERR_INPUT_SCAN_ERROR;
        }

        bool isSecondExist = false;
        ERROR_HANDLE(isObjExist(tree, second_object, &isSecondExist));
        printf("%d\n", isSecondExist);

        if (isSecondExist == false) {
            printf(BOLD CYAN "Попробуйте еще раз\n" RESET);
            continue;
        }

        if (make_comparison(tree, first_object, second_object) != TREE_ERR_SUCCESS) printf("Попробуйте еще\n");
        
        return TREE_ERR_SUCCESS;
    }
}
