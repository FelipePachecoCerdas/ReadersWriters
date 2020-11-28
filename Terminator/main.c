#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include  <stdio.h>
#include <stdlib.h>
#include  <string.h>

int main() {
    int ShmID;
    printf( "Ingrese el ID de la memoria compartida: ");
    scanf("%d",  &ShmID);

    shmctl(ShmID, IPC_RMID, NULL);
    printf("Server has removed its shared memory...\n");
    printf("Server exits...\n");
    exit(0);
}
