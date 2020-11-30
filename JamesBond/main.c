#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include  <stdio.h>
#include <stdlib.h>
#include  <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <zconf.h>

char * ARCHIVO_DE_CONTROL="/home/felipe/Desktop/Kraken/ReadersWriters/idCtl.txt";
int TAM_LINEA = 100;
char *MC_Ctl_ptr, *MC_ptr;

struct InfoBasica {
    int MC_Id, cantLineas,cantLectores,cantEscritores,cantEgoistas, acumuladoEgoistas, enJuego;
    sem_t semControl, semEgoista, semPrimerLector;
};

struct InfoBasica* infoBasica;

struct HiloProceso{
    pthread_t pid;
    char estado;
};

void printLineasArchivo() {
    printf("\n ***** LINEAS DEL ARCHIVO ***** \n");
    char *mem = MC_ptr;

    for (int i = 0; i < infoBasica->cantLineas; i++) {
        printf("[%i] %s\n", i, mem);
        mem += TAM_LINEA;
    }
}

void printProcesos(struct HiloProceso* procesos, int cantidad, char tipo, char *rw) {
    int cant = 0;
    for (int i = 0; i < cantidad; i++) {
        if (procesos[i].estado == tipo) {
            cant++; printf("[PID %lu]: %s\n", procesos[i].pid, rw);
        }
    }
    if (!cant) printf("%s\n", "(No hay)");
}

void printEscritores() {
    printf("\n ***** ESTADO DE ESCRITORES ***** \n");
    struct HiloProceso *procesosEscritores = MC_Ctl_ptr + sizeof(struct InfoBasica) + sizeof(struct HiloProceso) * 100;

    printf("\n + Escribiendo + \n");
    printProcesos(procesosEscritores, infoBasica->cantEscritores, 'E', "Escritor");
    printf("\n + Durmiendo + \n");
    printProcesos(procesosEscritores, infoBasica->cantEscritores, 'D', "Escritor");
    printf("\n + Bloqueados + \n");
    printProcesos(procesosEscritores, infoBasica->cantEscritores, 'B', "Escritor");
}

void printLectores() {
    printf("\n ***** ESTADO DE LECTORES ***** \n");
    struct HiloProceso *procesosLectores = MC_Ctl_ptr + sizeof(struct InfoBasica);

    printf("\n + Leyendo + \n");
    printProcesos(procesosLectores, infoBasica->cantLectores, 'L', "Lector");
    printf("\n + Durmiendo + \n");
    printProcesos(procesosLectores, infoBasica->cantLectores, 'D', "Lector");
    printf("\n + Bloqueados + \n");
    printProcesos(procesosLectores, infoBasica->cantLectores, 'B', "Lector");
}

void printEgoistas() {
    printf("\n ***** ESTADO DE LECTORES EGOISTAS ***** \n");
    struct HiloProceso *procesosEgoistas = MC_Ctl_ptr + sizeof(struct InfoBasica) + sizeof(struct HiloProceso) * 200;

    printf("\n + Leyendo y Borrando + \n");
    printProcesos(procesosEgoistas, infoBasica->cantEgoistas, 'X', "Lector Egoista");
    printf("\n + Durmiendo + \n");
    printProcesos(procesosEgoistas, infoBasica->cantEgoistas, 'D', "Lector Egoista");
    printf("\n + Bloqueados + \n");
    printProcesos(procesosEgoistas, infoBasica->cantEgoistas, 'B', "Lector Egoista");
}

void menuInfo() {
    printf("\n\n!Bienvenido al Programa Espia del Archivo Compartido!\n\n");

    printf("*** Opciones para los Estados de Procesos y del Archivo ***\n");
    printf("Realice alguna de las siguientes opciones mediante la digitacion de su correspondiente numero\n");
    printf("1. Estado del Archivo (lineas)\n");
    printf("2. Estado de los Escritores\n");
    printf("3. Estado de los Lectores\n");
    printf("4. Estado de los Lectores Egoistas\n");
}

int main() {
    int MC_Ctl_Id;
    FILE * fp = fopen (ARCHIVO_DE_CONTROL,"r");
    fscanf(fp, "%i", &MC_Ctl_Id);
    fclose (fp);

    MC_Ctl_ptr = shmat(MC_Ctl_Id, NULL, 0);
    infoBasica = (struct InfoBasica*) MC_Ctl_ptr;

    int MC_Id = infoBasica->MC_Id;
    MC_ptr = shmat(MC_Id, NULL, 0);

    int opcionEstado;
    for(;;) {
        menuInfo();
        printf("Digite la opcion: ");
        scanf("%i", &opcionEstado);

        if (opcionEstado <= 0 || opcionEstado >= 5) {
            printf("Opcion incorrecta. Intentelo de nuevo.\n");
            continue;
        }

        if (opcionEstado == 1)
            printLineasArchivo();
        else if (opcionEstado == 2)
            printEscritores();
        else if (opcionEstado == 3)
            printLectores();
        else
            printEgoistas();
    }

    return 0;
}


