#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include "minishell.h"
#include <time.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
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

 void ls_fun(){
    DIR *directorio;
    struct dirent *dirEntry;
    char ruta[RUTA];
    getcwd(ruta, RUTA);
    directorio = opendir(ruta);
    if(directorio == NULL){
        fprintf(stderr, "No puedo abrir el directorio %s. Error %s\n", ruta, strerror(errno));
        return;
    }
    while((dirEntry = readdir(directorio)) != NULL){
        printf("%s \n", dirEntry->d_name);
    }
    closedir(directorio);
 }

void stat_fun(const char *ruta){
    struct stat sb;
    if(opendir(ruta) == NULL) {
        printf("No es posiblo abrir el directorio %s", ruta);
        exit(EXIT_FAILURE);
    }
    if (lstat(ruta, &sb) == -1) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    printf("ID of containing device:  [%x,%x]\n", major(sb.st_dev), minor(sb.st_dev));

    printf("File type:                ");
    if (S_ISBLK(sb.st_mode)) {
        printf("block device\n");
    } else if (S_ISCHR(sb.st_mode)) {
        printf("character device\n");
    } else if (S_ISDIR(sb.st_mode)) {
        printf("directory\n");
    } else if (S_ISFIFO(sb.st_mode)) {
        printf("FIFO/pipe\n");
    } else if (S_ISLNK(sb.st_mode)) {
        printf("symlink\n");
    } else if (S_ISREG(sb.st_mode)) {
        printf("regular file\n");
    } else if (S_ISSOCK(sb.st_mode)) {
        printf("socket\n");
    } else {
        printf("unknown?\n");
    }


    printf("I-node number:            %ju\n", (uintmax_t) sb.st_ino);
    printf("Mode:                     %jo (octal)\n", (uintmax_t) sb.st_mode);

    printf("Link count:               %ju\n", (uintmax_t) sb.st_nlink);
    printf("Ownership:                UID=%ju   GID=%ju\n", (uintmax_t) sb.st_uid, (uintmax_t) sb.st_gid);

    printf("Preferred I/O block size: %jd bytes\n", (intmax_t) sb.st_blksize);
    printf("File size:                %jd bytes\n",(intmax_t) sb.st_size);
    printf("Blocks allocated:         %jd\n",(intmax_t) sb.st_blocks);

    printf("Last status change:       %s", ctime(&sb.st_ctime));
    printf("Last file access:         %s", ctime(&sb.st_atime));
    printf("Last file modification:   %s", ctime(&sb.st_mtime));

    exit(EXIT_SUCCESS);
}

void cat_fun(const char *ruta){
    struct stat sb;
    char buffer[1024];
    int tty;
    ssize_t bytes_read;

    if (lstat(ruta, &sb) == -1) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    if (S_ISDIR(sb.st_mode)) {
        printf("Es un directorio\n");
    } else if (S_ISREG(sb.st_mode)) {
        tty = open(ruta, O_RDONLY);
        if (tty == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        while ((bytes_read = read(tty, buffer, sizeof(buffer))) > 0) {
            write(STDOUT_FILENO, buffer, bytes_read);
        }

        if (bytes_read == -1) {
            perror("read");
        }

        close(tty);
    }
}

void unlink_fun(const char *ruta){
    if(unlink(ruta) == -1) perror("unlink");
}


void rename_fun(const char *old, const char *new){
    if(rename(old,new) == -1) perror("rename");
}

void find_fun(const char *ruta, const char *nombre_buscado){
    DIR *directorio;
    struct dirent *dirEntry;
    struct stat sb;
    char ruta_completa[1024];
    int encontrado = 0;
    int hay_subdirectorios = 0;
    directorio = opendir(ruta);
    if (directorio == NULL){
        fprintf(stderr, "No fue posible abrir el directorio %s. Error: %s\n",ruta, strerror(errno));
        return ;
    }
    while ((dirEntry = readdir(directorio)) != NULL){
        if (strcmp(dirEntry->d_name, ".") == 0 || strcmp(dirEntry->d_name, "..") == 0) {
            continue;
        }else if (snprintf(ruta_completa, sizeof(ruta_completa), "%s/%s", ruta, dirEntry->d_name) >= (int)sizeof(ruta_completa)) {
            fprintf(stderr, "Ruta demasiado larga, se omite: %s/%s\n", ruta, dirEntry->d_name);
            continue;
        }else if (lstat(ruta_completa, &sb) == -1) {
            perror("lstat");
            continue;
        }else if(S_ISDIR(sb.st_mode)) {
            hay_subdirectorios = 1;
        }else if (strcmp(dirEntry->d_name, nombre_buscado) == 0) {
            printf("Encontrado: %s\n", ruta_completa);
            encontrado = 1;
            break;
        }
    }

    closedir(directorio);

    if (encontrado) {
        return;
    }
    
    printf("No se encontro '%s' en '%s'.\n", nombre_buscado, ruta);
    if (hay_subdirectorios) {
        printf("Hay mas directorios para buscar :\n");

        directorio = opendir(ruta);
        if (directorio == NULL) {
            fprintf(stderr, "No se pudo reabrir el directorio %s. Error: %s\n", ruta, strerror(errno));
            return;
        }
        while ((dirEntry = readdir(directorio)) != NULL) {
            if (strcmp(dirEntry->d_name, ".") == 0 || strcmp(dirEntry->d_name, "..") == 0) {
                continue;
            }else if (snprintf(ruta_completa, sizeof(ruta_completa), "%s/%s", ruta, dirEntry->d_name) >= (int)sizeof(ruta_completa)) {
                continue;
            }else if (lstat(ruta_completa, &sb) == -1) {
                continue;
            }else if (S_ISDIR(sb.st_mode)) {
                printf("- %s\n", ruta_completa);
            }
        }
        closedir(directorio);
    }    
}