#include <time.h>
#include <stdio.h>
#include <string.h>
#include <sys/utsname.h>

void Cmd_date (char *arg[]) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char buffer[26];

    if (arg[0] == NULL) {
        strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
        printf("Fecha y hora actual: %s\n", buffer);
    } else if (strcmp(arg[0], "-d") == 0) {
        strftime(buffer, 26, "%Y-%m-%d", tm_info);
        printf("Fecha actual: %s\n", buffer);
    } else if (strcmp(arg[0], "-t") == 0) {
        strftime(buffer, 26, "%H:%M:%S", tm_info);
        printf("Hora actual: %s\n", buffer);
    } else {
        printf("Uso: date [-d] [-t]\n");
    }
}

void Cmd_sysinfo (char *arg[]) {
    struct utsname sysinfo;
    if (uname(&sysinfo) == -1) {
        perror("Imposible obtener información del sistema");
        return;
    }

    printf("Sistema operativo: %s\n", sysinfo.sysname);
    printf("Nombre del nodo: %s\n", sysinfo.nodename);
    printf("Versión del kernel: %s\n", sysinfo.release);
    printf("Versión del sistema: %s\n", sysinfo.version);
    printf("Arquitectura de la máquina: %s\n", sysinfo.machine);
}

void Cmd_help (char *arg[]) {
    printf("Implementar\n");
}