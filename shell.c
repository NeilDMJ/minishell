#include "minishell.h"
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
            stat_fun(args[1]);
        }
        
    }
    return 0;
}

//gcc -o shell shell.c lib/minishell.c