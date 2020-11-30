#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <memory.h>

int TAM_LINEA = 100, TRUE = 1, FALSE = 0;

struct InfoBasica {
    int MC_Id, cantLineas;
};
struct InfoBasica* infoBasica;

void *thread_function(void *);

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

int durMimir, durLeer, numLectores;
char * MC_ptr;


void leer() {
    char *mem, linea = malloc(TAM_LINEA);;
    int numLinea = 0, numLineaResp;

    do {
        numLineaResp = numLinea;

        for (; numLinea != numLineaResp; numLinea = (numLinea + 1) % infoBasica->cantLineas) {
            mem = MC_ptr + (TAM_LINEA * numLinea);
            memcpy(mem, linea, TAM_LINEA);
            if (!strcmp(linea, ""))
                break;
        }

        if (numLinea == numLineaResp)
            printf("Soy el thread %lu y no tengo nada que leer\n", pthread_self());
        else
            printf("Soy el thread %lu y leo: %s\n", pthread_self(), linea);

        sleep(durLeer);
    } while (TRUE);
}

int main() {
    int MC_Ctl_Id;
    FILE * fp = fopen ("/home/felipe/Desktop/Kraken/ReadersWriters/idCtl.txt","r");
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
    scanf("%d",  &durMimir);
    printf( "Ingrese la cantidad  de segundos para leer: ");
    scanf("%d",  &durLeer);

    pthread_t lectores[numLectores];
    for (int i = 0; i < numLectores; i++) {
        pthread_create(&lectores[i], NULL, leer, NULL );
        sleep(1);
    }

    for (int i = 0; i < numLectores; i++) {
        pthread_join(lectores[i], NULL );
    }


}


/*
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
void *thread_function(void *dummyPtr)
{
    while(1){
        printf("Hello from new thread - got %d\n", pthread_self());
        pthread_mutex_lock( &mutex1 );
        //Aqui estaria la sección critica
        pthread_mutex_unlock( &mutex1 );
        sleep(mimir);
    }
}
#pragma clang diagnostic pop
*/