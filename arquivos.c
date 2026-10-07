#include "arquivos.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include "listasimple.h"

static LISTASIMPLE openFiles;

void Cmd_trychdir(char *args[])
{
    int silent = 0;

    if (!strcmp(args[0], "-s")) {
        silent = 1;
        args++;
    }

    char* cadea;
    while ((cadea=*args) != NULL) {
        if (chdir(cadea)==-1) {
            if (!silent) {
                perror(cadea);
            }
        } else {
            if (!silent) {
                printf("%s Ok!\n", cadea);
            }
            return;
        }
        args++;
    }
    printf("Non puiden\n");
}

void ImprimirFichero(void *ptr) {
    FileEntry *f = (FileEntry *)ptr;
    int m = f->mode;

    long offset = lseek(f->df, 0, SEEK_CUR);
    char str_offset[32] = "  "; 

    if (offset != -1) sprintf(str_offset, "%ld", offset);

    printf("descriptor: %d, offset: (%s)-> %s %s%s%s%s%s\n", 
        f->df, 
        str_offset,
        f->name,
        (m & O_ACCMODE) == O_RDWR ? "O_RDWR" : ((m & O_ACCMODE) == O_WRONLY ? "O_WRONLY" : "O_RDONLY"),
        (m & O_CREAT)  ? " O_CREAT" : "",
        (m & O_EXCL)   ? " O_EXCL" : "",
        (m & O_APPEND) ? " O_APPEND" : "",
        (m & O_TRUNC)  ? " O_TRUNC" : ""
    );
}

void InitializeOpenFiles() {
    char *open_files[] = {"entrada estandar", "salida estandar", "error estandar"};
    for (int i = 0; i < 3; i++) {
        FileEntry *f = malloc(sizeof(FileEntry));
        f->df = i;
        f->mode = O_RDWR;
        strcpy(f->name, open_files[i]);
        f->name[MAXNAME-1] = '\0';
        AniadirElemento(openFiles, f);
    }
}

void Cmd_open (char * tr[])
{
    int i,df, mode=0;
    
    if (tr[0]==NULL) { /*no hay parametro*/
        ImprimirListaCompleta(openFiles, 0, ImprimirFichero);
        return;
    }

    for (i=1; tr[i]!=NULL; i++)
      if (!strcmp(tr[i],"cr")) mode|=O_CREAT;
      else if (!strcmp(tr[i],"ex")) mode|=O_EXCL;
      else if (!strcmp(tr[i],"ro")) mode|=O_RDONLY; 
      else if (!strcmp(tr[i],"wo")) mode|=O_WRONLY;
      else if (!strcmp(tr[i],"rw")) mode|=O_RDWR;
      else if (!strcmp(tr[i],"ap")) mode|=O_APPEND;
      else if (!strcmp(tr[i],"tr")) mode|=O_TRUNC; 
      else break;
      
    if ((df=open(tr[0],mode,0777))==-1)
        perror ("Imposible abrir fichero");
    else{
        FileEntry *f = malloc(sizeof(FileEntry));
        f->df = df;
        f->mode = mode;
        strncpy(f->name, tr[0], MAXNAME-1);
        f->name[MAXNAME-1] = '\0';
        if (AniadirElemento(openFiles, f)==-1) {
            perror("Imposible anadir entrada a la tabla ficheros abiertos");
            free(f);
        } else
            printf("Anadida entrada a la tabla ficheros abiertos. %s abierto con descriptor %d\n", tr[0], df);
    }
}

int CompFE(void *elemento_lista, void *descriptor_buscado) {

    FileEntry *f = (FileEntry *)elemento_lista;
    int *df = (int *)descriptor_buscado;
    
    if (f->df == *df) {
        return 0;
    }
    
    return 1;
}

void Cmd_close (char *tr[])
{ 
    int df;
    int fClose = 0;
    
    if (tr[0] == NULL || (df = atoi(tr[0])) < 0) {
        ImprimirListaCompleta(openFiles, 0, ImprimirFichero);
        return;
    }

    if (tr[1] != NULL && !strcmp(tr[1], "-f")) {
        fClose = 1; // Duda con respecto a implementar -f
        if (fClose) {
            printf("Se trata de forzar cierre del descriptor %d\n", df);
        }
    }

    
    if (close(df)==-1)
        perror("Imposible cerrar descriptor");
    else {
        if (BorrarElemento(openFiles, &df, CompFE)==-1)
            perror("Imposible borrar entrada de la tabla ficheros abiertos");
        else
            printf("Descriptor %d cerrado y eliminado de la tabla ficheros abiertos\n", df);
    }
}

void Cmd_listopen(char *tr[])
{
    ImprimirListaCompleta(openFiles, 0, ImprimirFichero);
}

void Cmd_lseek (char * tr[])
{
    int df;
    long pos, res;
    int ref = SEEK_SET;

    if (tr[0] == NULL || tr[1] == NULL) {
        printf("Parametros incorrectos\n");
        return;
    }

    df = atoi(tr[0]);
    if (df < 0 || (df == 0 && tr[0][0] != '0')) {
        printf("Parametros incorrectos\n");
        return;
    }

    pos = atol(tr[1]);

    if (tr[2] != NULL) {
        if (!strcmp(tr[2], "SEEK_SET")) {
            ref = SEEK_SET;
        } else if (!strcmp(tr[2], "SEEK_CUR")) {
            ref = SEEK_CUR;
        } else if (!strcmp(tr[2], "SEEK_END")) {
            ref = SEEK_END;
        } 
    }

    res = lseek(df, pos, ref);
    
    if (res == -1) {
        char err_msg[256];
        sprintf(err_msg, "Error al intentar posicionar el descriptor %d en el offset %ld", df, pos);
        perror(err_msg);
    } else {
        printf("Descriptor %d posicionado en %ld\n", df, res);
    }
}