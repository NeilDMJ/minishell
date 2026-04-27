#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <time.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <time.h>


#define MAX_ARGS 64
#define RUTA     255

void pwd_fun(void);
void cd_fun(const char *ruta);
void ls_fun(char **args);
void stat_fun(const char *ruta);
void cat_fun(const char *ruta);
void unlink_fun(const char *ruta);
void rename_fun(const char *old, const char *nuevo);
int  mkdir_fun(const char *cmd);
int  separar_cadena(char *cadena, char *args[], int max_args);
int  find_fun(const char *ruta, const char *nombre_buscado);
int fun_getifaddrs(void);
void date_fun(void);
#endif /* MINISHELL_H */