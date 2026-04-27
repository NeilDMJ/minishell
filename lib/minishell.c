#include "minishell.h"

#ifndef NI_NUMERICHOST
#define NI_NUMERICHOST 1
#endif

void pwd_fun(void)
{
    char ruta[RUTA];
    if (getcwd(ruta, RUTA) == NULL)
        perror("getcwd");
    else
        printf("%s\n", ruta);
}

void cd_fun(const char *ruta)
{
    if (ruta == NULL)
    {
        fprintf(stderr, "cd: falta el argumento\n");
        return;
    }
    if (chdir(ruta) == -1)
        perror("cd");
}

int mkdir_fun(const char *cmd)
{
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

int separar_cadena(char *cadena, char *args[], int max_args)
{
    int   i     = 0;
    char *token = strtok(cadena, " ");

    while (token != NULL && i < max_args - 1)
    {
        args[i++] = token;
        token     = strtok(NULL, " ");
    }
    args[i] = NULL;
    return i;
}

static void ls_print_long(const char *ruta_completa, const char *nombre)
{
    struct stat    sb;
    char           perms[11];
    char           tiempo[20];
    struct tm     *tm_info;
    struct passwd *pw;
    struct group  *gr;

    if (lstat(ruta_completa, &sb) == -1)
    {
        perror("lstat");
        return;
    }

    perms[0]  = S_ISDIR(sb.st_mode)  ? 'd' : (S_ISLNK(sb.st_mode) ? 'l' : '-');
    perms[1]  = (sb.st_mode & S_IRUSR) ? 'r' : '-';
    perms[2]  = (sb.st_mode & S_IWUSR) ? 'w' : '-';
    perms[3]  = (sb.st_mode & S_IXUSR) ? 'x' : '-';
    perms[4]  = (sb.st_mode & S_IRGRP) ? 'r' : '-';
    perms[5]  = (sb.st_mode & S_IWGRP) ? 'w' : '-';
    perms[6]  = (sb.st_mode & S_IXGRP) ? 'x' : '-';
    perms[7]  = (sb.st_mode & S_IROTH) ? 'r' : '-';
    perms[8]  = (sb.st_mode & S_IWOTH) ? 'w' : '-';
    perms[9]  = (sb.st_mode & S_IXOTH) ? 'x' : '-';
    perms[10] = '\0';

    tm_info = localtime(&sb.st_mtime);
    strftime(tiempo, sizeof(tiempo), "%b %d %H:%M", tm_info);

    pw = getpwuid(sb.st_uid);
    gr = getgrgid(sb.st_gid);

    printf("%s %2lu %-8s %-8s %6lld %s %s\n",
        perms,
        (unsigned long)sb.st_nlink,
        pw ? pw->pw_name : "?",
        gr ? gr->gr_name : "?",
        (long long)sb.st_size,
        tiempo,
        nombre);
}

void ls_fun(char **args)
{
    DIR           *directorio;
    struct dirent *dirEntry;
    char           ruta[RUTA];
    char           ruta_completa[RUTA + 256];
    int            flag_a = 0;
    int            flag_l = 0;
    int            flag_i = 0;

    for (int i = 1; args[i] != NULL; i++)
    {
        if (args[i][0] == '-')
        {
            for (int j = 1; args[i][j] != '\0'; j++)
            {
                if      (args[i][j] == 'a') flag_a = 1;
                else if (args[i][j] == 'l') flag_l = 1;
                else if (args[i][j] == 'i') flag_i = 1;
                else
                    fprintf(stderr, "ls: opcion desconocida: -%c\n", args[i][j]);
            }
        }
    }

    if (getcwd(ruta, RUTA) == NULL)
    {
        perror("getcwd");
        return;
    }

    directorio = opendir(ruta);
    if (directorio == NULL)
    {
        fprintf(stderr, "ls: no se puede abrir '%s': %s\n", ruta, strerror(errno));
        return;
    }

    while ((dirEntry = readdir(directorio)) != NULL)
    {
        if (!flag_a && dirEntry->d_name[0] == '.')
            continue;

        snprintf(ruta_completa, sizeof(ruta_completa), "%s/%s", ruta, dirEntry->d_name);

        if (flag_l)
        {
            if (flag_i)
                printf("%lu ", (unsigned long)dirEntry->d_ino);
            ls_print_long(ruta_completa, dirEntry->d_name);
        }
        else
        {
            if (flag_i)
                printf("%lu ", (unsigned long)dirEntry->d_ino);
            printf("%s\n", dirEntry->d_name);
        }
    }

    closedir(directorio);
}

void stat_fun(const char *ruta)
{
    struct stat sb;

    if (lstat(ruta, &sb) == -1)
    {
        perror("stat");
        return;
    }

    printf("Dispositivo:     [%x,%x]\n",   major(sb.st_dev), minor(sb.st_dev));
    printf("Tipo:            ");
    if      (S_ISBLK(sb.st_mode))  printf("dispositivo de bloque\n");
    else if (S_ISCHR(sb.st_mode))  printf("dispositivo de caracter\n");
    else if (S_ISDIR(sb.st_mode))  printf("directorio\n");
    else if (S_ISFIFO(sb.st_mode)) printf("FIFO/pipe\n");
    else if (S_ISLNK(sb.st_mode))  printf("enlace simbolico\n");
    else if (S_ISREG(sb.st_mode))  printf("archivo regular\n");
    else if (S_ISSOCK(sb.st_mode)) printf("socket\n");
    else                            printf("desconocido\n");

    printf("Inodo:           %ju\n",      (uintmax_t)sb.st_ino);
    printf("Modo:            %jo (octal)\n", (uintmax_t)sb.st_mode);
    printf("Enlaces:         %ju\n",      (uintmax_t)sb.st_nlink);
    printf("Propietario:     UID=%ju GID=%ju\n", (uintmax_t)sb.st_uid, (uintmax_t)sb.st_gid);
    printf("Tamano bloque:   %jd bytes\n",(intmax_t)sb.st_blksize);
    printf("Tamano archivo:  %jd bytes\n",(intmax_t)sb.st_size);
    printf("Bloques:         %jd\n",      (intmax_t)sb.st_blocks);
    printf("Ultimo cambio:   %s",         ctime(&sb.st_ctime));
    printf("Ultimo acceso:   %s",         ctime(&sb.st_atime));
    printf("Ultima modif:    %s",         ctime(&sb.st_mtime));
}

void cat_fun(const char *ruta)
{
    struct stat sb;
    char        buffer[1024];
    int         fd;
    ssize_t     bytes_read;

    if (lstat(ruta, &sb) == -1)
    {
        perror("cat");
        return;
    }

    if (S_ISDIR(sb.st_mode))
    {
        fprintf(stderr, "cat: %s: es un directorio\n", ruta);
        return;
    }

    fd = open(ruta, O_RDONLY);
    if (fd == -1)
    {
        perror("cat");
        return;
    }

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
        write(STDOUT_FILENO, buffer, bytes_read);

    if (bytes_read == -1)
        perror("cat");

    close(fd);
}

void unlink_fun(const char *ruta)
{
    if (unlink(ruta) == -1)
        perror("unlink");
}

void rename_fun(const char *old, const char *nuevo)
{
    if (rename(old, nuevo) == -1)
        perror("rename");
}

int find_fun(const char *ruta, const char *nombre_buscado)
{
    DIR           *dir;
    struct dirent *entry;
    struct stat    sb;
    char           ruta_completa[1024];

    dir = opendir(ruta);
    if (!dir)
    {
        fprintf(stderr, "find: no se pudo abrir '%s': %s\n", ruta, strerror(errno));
        return 0;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.')
            continue;

        if (snprintf(ruta_completa, sizeof(ruta_completa),
                "%s/%s", ruta, entry->d_name) >= (int)sizeof(ruta_completa))
            continue;

        if (strcmp(entry->d_name, nombre_buscado) == 0)
        {
            printf("Encontrado: %s\n", ruta_completa);
            closedir(dir);
            return 1;
        }

        if (lstat(ruta_completa, &sb) == 0 && S_ISDIR(sb.st_mode))
            if (find_fun(ruta_completa, nombre_buscado))
            {
                closedir(dir);
                return 1;
            }
    }

    closedir(dir);
    return 0;
}

int fun_getifaddrs(void)
{
    struct ifaddrs *ifaddr, *ifa;
    char  host[INET6_ADDRSTRLEN];

    if (getifaddrs(&ifaddr) == -1)
    {
        perror("getifaddrs");
        return -1;
    }

    printf("Interfaces de red:\n");
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
            continue;

        int family = ifa->ifa_addr->sa_family;

        //Verificamos las familias de direcciones IPV4 e IPV6
        if (family == AF_INET || family == AF_INET6)
        {
            int s = getnameinfo(ifa->ifa_addr,
                (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6),
                host, INET6_ADDRSTRLEN, NULL, 0, NI_NUMERICHOST);
            if (s != 0)
            {
                perror("getnameinfo");
                continue;
            }
            printf("%s: ", ifa->ifa_name);
            printf("%s\n", host);
        }
    }

    freeifaddrs(ifaddr);
    return 0;
}

void date_fun(void)
{
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char buffer[64];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("%s CST", buffer);
}

void who_fun(){
    system("who");
}

void uname_fun(){
    struct utsname info;
    uname(&info);
    printf("Sistema: %s\n", info.sysname);
    printf("Nodo: %s\n", info.nodename);
    printf("Release: %s\n", info.release);
    printf("Version: %s\n", info.version);
    printf("Arquitectura: %s\n", info.machine);
}

void wall_fun(char **args)
{
    struct utmp  *entry;
    char          tty_path[64];
    char          mensaje[1024] = {0};
    int           fd;

    if (args[1] == NULL)
    {
        fprintf(stderr, "wall: falta el mensaje\n");
        return;
    }
    for (int i = 1; args[i] != NULL; i++)
    {
        if (i > 1)
            strncat(mensaje, " ", sizeof(mensaje) - strlen(mensaje) - 1);
        strncat(mensaje, args[i], sizeof(mensaje) - strlen(mensaje) - 1);
    }
    strncat(mensaje, "\n", sizeof(mensaje) - strlen(mensaje) - 1);

    setutent();
    while ((entry = getutent()) != NULL)
    {
        if (entry->ut_type != USER_PROCESS)
            continue;

        snprintf(tty_path, sizeof(tty_path), "/dev/%s", entry->ut_line);

        fd = open(tty_path, O_WRONLY | O_NOCTTY);
        if (fd == -1)
        {
            fprintf(stderr, "wall: no se puede escribir en %s: %s\n",
                    tty_path, strerror(errno));
            continue;
        }
        write(fd, "\n[Broadcast]: ", 14);
        write(fd, mensaje, strlen(mensaje));
        close(fd);
    }
    endutent();
}

void mac_fun(char **args)
{
    struct ifaddrs     *ifaddr;
    struct ifaddrs     *ifa;
    struct sockaddr_ll *s;
    unsigned char      *mac;

    (void)args;

    if (getifaddrs(&ifaddr) == -1)
    {
        perror("mac: getifaddrs");
        return;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
            continue;
        if (ifa->ifa_addr->sa_family != AF_PACKET)
            continue;

        s   = (struct sockaddr_ll *)ifa->ifa_addr;
        mac = s->sll_addr;

        if (mac[0] == 0 && mac[1] == 0 && mac[2] == 0 &&
            mac[3] == 0 && mac[4] == 0 && mac[5] == 0)
            continue;

        printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
               mac[0], mac[1], mac[2],
               mac[3], mac[4], mac[5]);
    }

    freeifaddrs(ifaddr);
}