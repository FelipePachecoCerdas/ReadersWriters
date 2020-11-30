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
char * ARCHIVO_DE_CONTROL="/home/felipe/Desktop/Kraken/ReadersWriters/idCtl.txt";

struct InfoBasica {
    int MC_Id, cantLineas,cantLectores,cantEscritores,cantEgoistas, acumuladoEgoistas, enJuego;
    sem_t semControl, semEgoista, semPrimerLector;
};

struct HiloProceso{
    pthread_t pid;
    char estado;
};

struct InfoBasica* infoBasica;
struct HiloProceso* procesosEscritores;

void *thread_function(void *);

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

int durDormir, durEscribir, numEscritores;
char * MC_ptr;

char *getTime() {
    char *s = malloc(100);
    time_t tiempo;
    struct tm * infoTiempo;
    time( &tiempo );
    infoTiempo = localtime( &tiempo );
    snprintf(s, 100, "%02d:%02d:%02d", infoTiempo->tm_hour, infoTiempo->tm_min, infoTiempo->tm_sec);
    return s;
}

void escribir(int i) {
    procesosEscritores[i].pid = pthread_self();

    char *mem, *linea = malloc(TAM_LINEA);
    int numLinea = 0, numLineaResp, valorPrimerLector;

    do {
        procesosEscritores[i].estado = 'B';
        sem_wait(&infoBasica->semControl);

        //region critica
        numLineaResp = numLinea;
        procesosEscritores[i].estado = 'E';

        sem_getvalue(&infoBasica->semPrimerLector, &valorPrimerLector);
        if( valorPrimerLector == 0)
            sem_post(&infoBasica->semPrimerLector);


        do {
            mem = MC_ptr + (TAM_LINEA * numLinea);
            memcpy(linea,mem , TAM_LINEA);

            if (!strcmp(linea, ""))
                break;
            numLinea = (numLinea + 1) % infoBasica->cantLineas;
        } while (numLinea != numLineaResp);
        char *mensajeBitacora = malloc(1000);

        if (numLinea == numLineaResp && strcmp(linea, ""))
            snprintf(mensajeBitacora, 1000, "[%s] PID: %lu (Escritor) - No escribe porque el archivo esta lleno\n", getTime(), pthread_self());
        else {
            sleep(durEscribir);
            time_t t = time(NULL);
            struct tm *tm = localtime(&t);
            char * fecha = asctime(tm);
            fecha[strlen(fecha)-1] = '\0';

            snprintf(linea,100,"PID: %lu | FECHA: %s | LINEA: %i",pthread_self(),fecha,numLinea);
            memcpy(mem,linea,TAM_LINEA);
            snprintf(mensajeBitacora, 1000, "[%s] PID: %lu (Escritor) - Escribe \"%s\"\n", getTime(), pthread_self(), linea);
        }
        printf("%s", mensajeBitacora);

        sem_post(&infoBasica->semControl);

        procesosEscritores[i].estado = 'D';
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

    printf( "Ingrese la cantidad de escritores deseados: ");
    scanf("%d",  &numEscritores);
    printf( "Ingrese la cantidad  de segundos para dormir: ");
    scanf("%d",  &durDormir);
    printf( "Ingrese la cantidad  de segundos para escribir: ");
    scanf("%d",  &durEscribir);

    infoBasica->cantEscritores = numEscritores;
    procesosEscritores = MC_Ctl_ptr + sizeof(struct InfoBasica) + sizeof(struct HiloProceso) * 100;

    pthread_t hilosEscritores[numEscritores];
    for (int i = 0; i < numEscritores; i++) {
        pthread_create(&hilosEscritores[i], NULL, escribir, i);
        sleep(1);
    }
    while(infoBasica->enJuego)
        sleep(1);

    for (int i = 0; i < numEscritores; i++) {
        pthread_cancel(hilosEscritores[i]);
    }

}