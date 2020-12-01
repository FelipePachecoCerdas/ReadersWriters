#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include  <stdio.h>
#include <stdlib.h>
#include  <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <zconf.h>

#define TRUE 1
#define FALSE 0

int TAM_LINEA=100;

char * ARCHIVO_DE_CONTROL="/home/jdtm23/Documents/ReadersWriters/idCtl.txt";
char * ARCHIVO_BITACORA = "/home/jdtm23/Documents/ReadersWriters/bitacora.txt";

struct InfoBasica {
    int MC_Id, cantLineas,cantLectores,cantEscritores,cantEgoistas, acumuladoEgoistas, enJuego, primerLectorTermino;
    sem_t semControl, semEgoista, semPrimerLector, semContadorModificacion, semContadorNoEgoistas;
};
struct HiloProceso{
    pthread_t pid;
    char estado;
};

char *getTime() {
    char *s = malloc(100);
    time_t tiempo;
    struct tm * infoTiempo;
    time( &tiempo );
    infoTiempo = localtime( &tiempo );
    snprintf(s, 100, "%02d:%02d:%02d", infoTiempo->tm_hour, infoTiempo->tm_min, infoTiempo->tm_sec);
    return s;
}