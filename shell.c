#include "lib/minishell.h"
#include <string.h>
#include <stdio.h>

int main() {
    char cmd[255];
    char *args[MAX_ARGS];

    while(1){
        printf("\n> ");
        if (fgets(cmd, sizeof(cmd), stdin) == NULL) 
            return 1;

        cmd[strcspn(cmd, "\n")] = '\0';

        int argc = separar_cadena(cmd, args, MAX_ARGS);
        if (argc == 0)
            continue;

        if (strcmp(args[0], "pwd") == 0){
            pwd_fun();
        }
        else if (strcmp(args[0], "mkdir") == 0){
            if (argc < 2) {
                fprintf(stderr, "Uso: mkdir <nombre_directorio>\n");
                continue;
            }
            mkdir_fun(args[1]);
        }
        else if (strcmp(args[0], "cd") == 0){
            if (argc < 2) {
                fprintf(stderr, "Uso: cd <ruta>\n");
                continue;
            }
            cd_fun(args[1]);
        }
        else if (strcmp(args[0], "ls") == 0){
            if (argc < 2) {
                ls_fun(); 
            } 
        }
        else if(strcmp(args[0], "clear") == 0 ){
            //limpiar
            system("clear");
        }
        else if(strcmp(args[0], "exit") == 0){
            break;
        }
        else if(strcmp(args[0], "ls") == 0){
            ls_fun();
        }
        else if(strcmp(args[0], "stat") == 0){
                if (argc < 2) {
                    fprintf(stderr, "Uso: stat <ruta>\n");
                    continue;
                }
            stat_fun(args[1]);
        }
        else if(strcmp(args[0], "cat") == 0){
            if (argc < 2) {
                fprintf(stderr, "Uso: cat <ruta>\n");
                continue;
            }
            cat_fun(args[1]);
        }
        else if(strcmp(args[0],"unlink") == 0){
            //validacion
            if (argc < 2) {
                fprintf(stderr, "Uso: unlink <ruta>\n");
                continue;
            }
            unlink_fun(args[1]);
        }
        else if(strcmp(args[0], "rename")== 0){
            //validacion 
            if (argc < 3) {
                fprintf(stderr, "Uso: rename <nombre_antiguo> <nombre_nuevo>\n");
                continue;
            }
            rename_fun(args[1], args[2]);
        }
        
    }
    return 0;
}

//gcc -I lib/ shell.c lib/minishell.c -o minishell