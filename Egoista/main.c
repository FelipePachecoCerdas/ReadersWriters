#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>

void *thread_function(void *);
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int mimir = 0;
int durarEscribir = 0;
int tamano = 10;

struct mensaje {
    float pid;
    int fechaHora; //Todavia no se que tipo de dato seria xd
    int linea;
    char msj[1024];
};
struct Memoria {
    int len;
    struct mensaje msj[];
};
struct Memoria *createMemory(struct Memoria *s,int tamano)
{
    // Allocating memory according to user provided
    // array of characters
    s =
            malloc( sizeof(*s) + sizeof(struct mensaje ) * tamano);


    s->msj[tamano];
    // Assigning size according to size of stud_name
    // which is a copy of user provided array a[].
    s->len =
            (sizeof(*s) + sizeof(struct mensaje ) * tamano);

    return s;
}

main() {

    key_t          ShmKEY;
    int            ShmID;
    //struct mensaje  *ShmPTR;

    printf( "Ingrese el ID de la memoria compartida: ");
    scanf("%d",  &ShmID);
    int tamanoMemory;

    printf( "Ingrese el tamaño deseado para la memoria compartida: ");
    scanf("%d",  &tamanoMemory);

    struct Memoria *ShmPTR = createMemory(ShmPTR, tamanoMemory);

    ShmPTR = (struct Memoria *) shmat(ShmID, NULL, 0);
    if ((int) ShmPTR == -1) {
        printf("*** shmat error (client) ***\n");
        exit(1);
    }
    printf("   Client has attached the shared memory...\n");

    printf(ShmPTR->msj[0].msj);


    /*
    int numThreads;


    printf( "Ingrese la cantidad de escritores deseados: ");
    scanf("%d",  &numThreads);

    printf( "Ingrese la cantidad  de segundos para mimir: ");
    scanf("%d",  &mimir);

    printf( "Ingrese la cantidad  de segundos para escribir: ");
    scanf("%d",  &durarEscribir);
    pthread_t thread_id[numThreads];


    int i, j;

    for(i=0; i < numThreads; i++)
    {
        pthread_create( &thread_id[i], NULL, thread_function, NULL );
    }

    for(j=0; j < numThreads; j++)
    {
        pthread_join( thread_id[j], NULL);
    }*/

    /* Now that all threads are complete I can print the final result.     */
    /* Without the join I could be printing a value before all the threads */
    /* have been completed.                                                */

}


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
