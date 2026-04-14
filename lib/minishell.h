#ifndef MINISHELL_H
#define MINISHELL_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#define MAX_ARGS 64

void pwd_fun();

void cd_fun(const char *ruta);

int mkdir_fun(const char *cmd);

int separar_cadena(char *cadena, char *args[], int max_args);

void ls_fun();

void stat_fun(const char *ruta);

void cat_fun(const char *ruta);

<<<<<<< HEAD
void rename_fun(const char *old_name, const char *new_name);
=======
void unlink_fun(const char *ruta);

void rename_fun(const char *old, const char *new);
>>>>>>> 465003834a44f3b30480398155210c584dde0076

#endif /* MINISHELL_H */