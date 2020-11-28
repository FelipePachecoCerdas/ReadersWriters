#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include  <stdio.h>
#include <stdlib.h>
#include  <string.h>

struct Mensaje {
    float pid;
    int fechaHora; //Todavia no se que tipo de dato seria xd
    int linea;
    char msj[1024];
};

struct Memoria {
    int len;
    struct Mensaje msj[];
};

struct Memoria *createMemory(struct Memoria *s,int tamano)
{
    // Allocating memory according to user provided
    // array of characters
    s =
            malloc( sizeof(*s) + sizeof(struct Mensaje ) * tamano);


    s->msj[tamano];
    // Assigning size according to size of stud_name
    // which is a copy of user provided array a[].
    s->len =
            (sizeof(*s) + sizeof(struct Mensaje ) * tamano);

    return s;
}

int main() {
    key_t ShmKEY;
    int ShmID;
    //struct Mensaje  *ShmPTR;
    int tamanoMemory;

    printf( "Ingrese el tamaño deseado para la memoria compartida: ");
    scanf("%d",  &tamanoMemory);

    struct Memoria *ShmPTR = createMemory(ShmPTR, tamanoMemory);


    ShmKEY = ftok(".", 'x');
    ShmID = shmget(ShmKEY, ShmPTR->len, IPC_CREAT | 0666);
    if (ShmID < 0) {
        printf("shmget error\n");
        exit(1);
    }

    printf( "Shared memory id is: %d",ShmID);


    return 0;
}
