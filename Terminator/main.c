#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include  <stdio.h>
#include <stdlib.h>
#include  <string.h>
#include <semaphore.h>
#include <unistd.h>

char * ARCHIVO_DE_CONTROL="/home/jdtm23/Documents/ReadersWriters/idCtl.txt";

struct InfoBasica {
    int MC_Id, cantLineas,cantLectores,cantEscritores,cantEgoistas, acumuladoEgoistas, enJuego;
    sem_t semControl, semEgoista, semPrimerLector;
};
int main() {
    int MC_Ctl_Id;
    FILE * fp = fopen (ARCHIVO_DE_CONTROL,"r");
    fscanf(fp, "%i", &MC_Ctl_Id);
    fclose (fp);

    char * MC_Ctl_ptr = shmat(MC_Ctl_Id, NULL, 0);
    struct InfoBasica* infoBasica = (struct InfoBasica*) MC_Ctl_ptr;
    //printf("SOY %i %i \n", infoBasica->MC_Id, infoBasica->cantLineas);

    int MC_Id = infoBasica->MC_Id;

    infoBasica->enJuego = 0;
    sleep(3);

    shmctl(MC_Id, IPC_RMID, NULL);
    shmctl(MC_Ctl_Id, IPC_RMID, NULL);
    printf("Server has removed its shared memory...\n");
    printf("Server exits...\n");
    exit(0);
}
