#ifndef MINISHELL_H
#define MINISHELL_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>
<<<<<<< HEAD

=======
#include <errno.h>
#include <sys/stat.h>
>>>>>>> 6a575be7465525c3879249bbc614f8ad43ac5d8b
#define MAX_ARGS 64

void pwd_fun();

void cd_fun(const char *ruta);

int mkdir_fun(const char *cmd);

int separar_cadena(char *cadena, char *args[], int max_args);

<<<<<<< HEAD
void ls_fun(const char *dir);
=======
void ls_fun();

void stat_fun(const char *ruta);

void cat_fun(const char *ruta);

>>>>>>> 6a575be7465525c3879249bbc614f8ad43ac5d8b

#endif /* MINISHELL_H */