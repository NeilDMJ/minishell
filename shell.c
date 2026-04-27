#include "lib/minishell.h"

int main(void)
{
    char  cmd[255];
    char *args[MAX_ARGS];

    while (1)
    {
        printf("\n> ");
        fflush(stdout);

        if (fgets(cmd, sizeof(cmd), stdin) == NULL)
            break;

        cmd[strcspn(cmd, "\n")] = '\0';

        int argc = separar_cadena(cmd, args, MAX_ARGS);
        if (argc == 0)
            continue;

        if (strcmp(args[0], "pwd") == 0)
            pwd_fun();

        else if (strcmp(args[0], "cd") == 0)
        {
            if (argc < 2) fprintf(stderr, "Uso: cd <ruta>\n");
            else          cd_fun(args[1]);
        }
        else if (strcmp(args[0], "mkdir") == 0)
        {
            if (argc < 2) fprintf(stderr, "Uso: mkdir <nombre>\n");
            else          mkdir_fun(args[1]);
        }
        else if (strcmp(args[0], "ls") == 0)
            ls_fun(args);

        else if (strcmp(args[0], "stat") == 0)
        {
            if (argc < 2) fprintf(stderr, "Uso: stat <ruta>\n");
            else          stat_fun(args[1]);
        }
        else if (strcmp(args[0], "cat") == 0)
        {
            if (argc < 2) fprintf(stderr, "Uso: cat <ruta>\n");
            else          cat_fun(args[1]);
        }
        else if (strcmp(args[0], "unlink") == 0)
        {
            if (argc < 2) fprintf(stderr, "Uso: unlink <ruta>\n");
            else          unlink_fun(args[1]);
        }
        else if (strcmp(args[0], "rename") == 0)
        {
            if (argc < 3) fprintf(stderr, "Uso: rename <antiguo> <nuevo>\n");
            else          rename_fun(args[1], args[2]);
        }
        else if (strcmp(args[0], "find") == 0)
        {
            if (argc < 3) fprintf(stderr, "Uso: find <ruta> <nombre>\n");
            else          find_fun(args[1], args[2]);
        }
        else if(strcmp(args[0], "free") == 0){
            system("free -h");
        }
        else if(strcmp(args[0], "ip") == 0){
            fun_getifaddrs();
        }
        else if(strcmp(args[0], "date") == 0){
            date_fun();
        }
        else if (strcmp(args[0], "clear") == 0)
            system("clear");

        else if (strcmp(args[0], "exit") == 0)
            break;

        else
            fprintf(stderr, "%s: comando no encontrado\n", args[0]);
    }

    return 0;
}
//gcc -I lib/ shell.c lib/minishell.c -o minishell