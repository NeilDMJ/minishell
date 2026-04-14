#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "minishell.h"
#define RUTA 255

void pwd_fun()
{
    char ruta[RUTA];
    getcwd(ruta, RUTA);
    printf("%s\n", ruta);
}

//Dirigirse a una ruta especifica
void cd_fun(const char *ruta ){
    if (ruta == NULL) printf("Falta el nombre del directorio");
    if(chdir(ruta) == -1) printf("No existe la ruta");
}

//separar
int separar_cadena(char *cadena, char *args[], int max_args){
    int i = 0;

    //espacio
    char *token = strtok(cadena, " ");
    while (token != NULL && i < max_args - 1)
    {
        args[i++] = token;
        token = strtok(NULL, " ");
    }

    args[i] = NULL;
    return i;
}


//directorio
int mkdir_fun(const char *cmd){
    if (cmd == NULL)
    {
        fprintf(stderr, "mkdir: falta el nombre del directorio\n");
        return -1;
    }

    if (mkdir(cmd, 0777) != 0)
    {
        perror("mkdir");
        return -1;
    }

    return 0;
}

//funcion ls con opendir, readdir y closedir, reusando getcwd
void ls_fun(const char *dir){
    char ruta[RUTA];
    if (dir == NULL) {
        getcwd(ruta, RUTA);
        dir = ruta;
    }

    DIR *d = opendir(dir);
    if (d == NULL) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(d)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(d);
}