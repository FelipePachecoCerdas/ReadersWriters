#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <memory.h>
#include <semaphore.h>

int TAM_LINEA = 100, TRUE = 1, FALSE = 0;
char * ARCHIVO_DE_CONTROL="/home/jdtm23/Documents/ReadersWriters/idCtl.txt";

struct InfoBasica {
    int MC_Id, cantLineas,cantLectores,cantEscritores,cantEgoistas, acumuladoEgoistas, enJuego;
    sem_t semControl, semEgoista, semPrimerLector;
};

struct HiloProceso{
    pthread_t pid;
    char estado;
};

struct InfoBasica* infoBasica;
struct HiloProceso* procesosLectores;

void *thread_function(void *);

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

int durDormir, durLeer, numLectores;
char * MC_ptr;


void leer(int i) {
    procesosLectores[i].pid = pthread_self();

    char *mem, *linea = malloc(TAM_LINEA);
    int numLinea = 0, numLineaResp;

    do {
        procesosLectores[i].estado = 'B';
        //wait
        numLineaResp = numLinea;
        procesosLectores[i].estado = 'L';
        sleep(durLeer);

        do {
            mem = MC_ptr + (TAM_LINEA * numLinea);
            memcpy(linea,mem , TAM_LINEA);
            printf("Esta es la linea: %s %i\n",linea,numLinea);
            if (strcmp(linea, ""))
                break;
            numLinea = (numLinea + 1) % infoBasica->cantLineas;
        } while (numLinea != numLineaResp);

        if (numLinea == numLineaResp && !strcmp(linea, ""))
            printf("Soy el thread %lu y no tengo nada que leer\n", pthread_self());
        else
            printf("Soy el thread %lu y leo: %s\n", pthread_self(), linea);

        procesosLectores[i].estado = 'D';
        sleep(durDormir);

        numLinea = (numLinea + 1) % infoBasica->cantLineas;
    } while (TRUE);
}

int main() {
    int MC_Ctl_Id;
    FILE * fp = fopen (ARCHIVO_DE_CONTROL,"r");
    fscanf(fp, "%i", &MC_Ctl_Id);
    fclose (fp);

    char * MC_Ctl_ptr = shmat(MC_Ctl_Id, NULL, 0);
    infoBasica = (struct InfoBasica*) MC_Ctl_ptr;
    //printf("SOY %i %i \n", infoBasica->MC_Id, infoBasica->cantLineas);

    int MC_Id = infoBasica->MC_Id;
    MC_ptr = shmat(MC_Id, NULL, 0);

    if ((int) MC_ptr == -1) {
        printf("*** shmat error (client) ***\n");
        exit(1);
    }
    printf("   Client has attached the shared memory...\n");

    printf( "Ingrese la cantidad de lectores deseados: ");
    scanf("%d",  &numLectores);
    printf( "Ingrese la cantidad  de segundos para dormir: ");
    scanf("%d",  &durDormir);
    printf( "Ingrese la cantidad  de segundos para leer: ");
    scanf("%d",  &durLeer);

    infoBasica->cantLectores = numLectores;
    procesosLectores = MC_Ctl_ptr + sizeof(struct InfoBasica);

    pthread_t hilosLectores[numLectores];
    for (int i = 0; i < numLectores; i++) {
        pthread_create(&hilosLectores[i], NULL, leer, i);
        sleep(1);
    }
    while(infoBasica->enJuego)
        sleep(1);

    for (int i = 0; i < numLectores; i++) {
        pthread_cancel(hilosLectores[i]);
    }

}