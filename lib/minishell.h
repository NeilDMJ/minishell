#ifndef MINISHELL_H
#define MINISHELL_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>

#define MAX_ARGS 64

void pwd_fun();

void cd_fun(const char *ruta);

int mkdir_fun(const char *cmd);

int separar_cadena(char *cadena, char *args[], int max_args);

void ls_fun(const char *dir);

#endif /* MINISHELL_H */