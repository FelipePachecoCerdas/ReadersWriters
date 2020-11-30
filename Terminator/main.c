#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include  <stdio.h>
#include <stdlib.h>
#include  <string.h>
#include <semaphore.h>
#include <unistd.h>

char * ARCHIVO_DE_CONTROL = "/home/felipe/Desktop/Kraken/ReadersWriters/idCtl.txt";
char * ARCHIVO_BITACORA = "/home/felipe/Desktop/Kraken/ReadersWriters/bitacora.txt";

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

    int MC_Id = infoBasica->MC_Id;

    infoBasica->enJuego = 0;
    sleep(3);

    shmctl(MC_Id, IPC_RMID, NULL);
    shmctl(MC_Ctl_Id, IPC_RMID, NULL);
    printf("Se han terminado todos los procesos e hilos de ejecucion del proyecto, ademas se han liberado las memorias compartidas y el archivo fisico compartido\n");
    printf("Consulte el archivo de bitacora en \"%s\" para obtener toda la informacion relativa a la ejecucion\n", ARCHIVO_BITACORA);
    exit(0);
}
