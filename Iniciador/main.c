#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include  <stdio.h>
#include <stdlib.h>
#include  <string.h>

int TAM_LINEA = 100;
/*
struct Mensaje {
    float pid;
    int fechaHora; //Todavia no se que tipo de dato seria xd
    int linea;
    char msj[1024];
};*/

struct InfoBasica {
    int MC_Id, cantLineas;
};

int main() {
    int cantLineas;
    printf( "Ingrese la cantidad de lineas para la memoria compartida: ");
    scanf("%d",  &cantLineas);

    key_t MC_Llave = ftok(".", 'x');
    int MC_Id = shmget(MC_Llave, TAM_LINEA * cantLineas, IPC_CREAT | 0666);

    if (MC_Id < 0) {
        printf("shmget error\n");
        exit(1);
    }

    printf( "Shared memory id is: %d", MC_Id);

    char * MC_ptr = shmat(MC_Id, NULL, 0);
    for (int i = 0; i < TAM_LINEA * cantLineas; i++) {
        MC_ptr[i] = '\0';
    }

    char * mem = MC_ptr;
    for (int i = 0; i < cantLineas; i++) {
        char * linea = malloc(100);
        snprintf(linea, 100, "Hola soy %i", i);

        memcpy(mem, linea,  strlen(linea));
        mem += TAM_LINEA;
    }

    key_t MC_Ctl_Llave = ftok(".", 'HOLA');
    int MC_Ctl_Id = shmget(MC_Ctl_Llave, 1000, IPC_CREAT | 0666);
    char * MC_Ctl_ptr = shmat(MC_Ctl_Id, NULL, 0);

    struct InfoBasica* infoBasica = (struct InfoBasica*) MC_Ctl_ptr;
    infoBasica->MC_Id = MC_Id;
    infoBasica->cantLineas = cantLineas;

    FILE * fp = fopen ("/home/felipe/Desktop/Kraken/ReadersWriters/idCtl.txt","w");
    fprintf(fp, "%i", MC_Ctl_Id);
    fclose (fp);

    printf("\nShared CONTROL memory id is: %d\n", MC_Ctl_Id);

    return 0;
}
