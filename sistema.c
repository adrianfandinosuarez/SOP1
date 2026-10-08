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

void Cmd_help(char *tr[]) {
    if (tr[0] == NULL) {
        printf("Comandos disponibles:\n");
        printf("exit bye date pid authors sysinfo help chdir open close listopen dup lseek readstr writestr makefile makedir delete deltree listfile list\n");
        return;
    }

    if (!strcmp(tr[0], "exit") || !strcmp(tr[0], "bye")) {
        printf("%s\t\t\tTermina la ejecucion del shell\n", tr[0]);
    } else if (!strcmp(tr[0], "date")) {
        printf("date [-d|-t]\t\tMuestra la fecha y/o la hora actual\n");
    } else if (!strcmp(tr[0], "pid")) {
        printf("pid [-p]\t\tMuestra el PID del shell o de su proceso padre\n");
    } else if (!strcmp(tr[0], "authors")) {
        printf("authors [-l|-n]\t\tMuestra los nombres y/o logins de los autores\n");
    } else if (!strcmp(tr[0], "sysinfo")) {
        printf("sysinfo\t\t\tMuestra informacion de la maquina\n");
    } else if (!strcmp(tr[0], "help")) {
        printf("help [cmd]\t\tMuestra ayuda sobre los comandos\n");
    } else if (!strcmp(tr[0], "chdir")) {
        printf("chdir [dir]\t\tCambia el directorio actual de trabajo\n");
    } else if (!strcmp(tr[0], "open")) {
        printf("open file m1 m2...\tAbre el fichero file.\n\t\t\tModos: cr, ap, ex, ro, rw, wo, tr\n");
    } else if (!strcmp(tr[0], "close")) {
        printf("close df [-f]\t\tCierra el descriptor df\n");
    } else if (!strcmp(tr[0], "listopen")) {
        printf("listopen [n]\t\tLista los ficheros abiertos (al menos n) del shell\n");
    } else if (!strcmp(tr[0], "dup")) {
        printf("dup df\t\t\tDuplica el descriptor de fichero df\n");
    } else if (!strcmp(tr[0], "lseek")) {
        printf("lseek df pos ref\tPosiciona el offset del fichero.\n\t\t\tref: SEEK_SET, SEEK_CUR, SEEK_END\n");
    } else if (!strcmp(tr[0], "readstr")) {
        printf("readstr df cont\t\tLee cont bytes del fichero con descriptor df\n\t\t\ty los muestra en pantalla\n");
    } else if (!strcmp(tr[0], "writestr")) {
        printf("writestr df string\tEscribe la cadena en el fichero descrito por df\n");
    } else if (!strcmp(tr[0], "makefile")) {
        printf("makefile name\t\tCrea un fichero vacio de nombre name\n");
    } else if (!strcmp(tr[0], "makedir")) {
        printf("makedir name\t\tCrea un directorio de nombre name\n");
    } else if (!strcmp(tr[0], "delete")) {
        printf("delete name1 name2...\tBorra ficheros o directorios vacios\n");
    } else if (!strcmp(tr[0], "deltree")) {
        printf("deltree name1 name2...\tBorra ficheros o directorios recursivamente\n");
    } else if (!strcmp(tr[0], "listfile")) {
        printf("listfile [-long][-link][-acc] name1 name2...\n");
        printf("\t\t\tLista ficheros;\n");
        printf("\t\t\t-long: listado largo\n");
        printf("\t\t\t-acc:  acesstime\n");
        printf("\t\t\t-link: si es enlace simbolico, el path contenido\n");
    } else if (!strcmp(tr[0], "list")) {
        printf("list [-reca] [-recb] [-hid][-long][-link][-acc] n1 n2...\n");
        printf("\t\t\tLista contenidos de directorios\n");
    } else {
        printf("Comando no encontrado. Teclee 'help' para ver la lista de comandos disponibles.\n");
    }
}