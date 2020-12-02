#include "../util.c"

char *MC_Ctl_ptr, *MC_ptr;

struct InfoBasica* infoBasica;

void printLineasArchivo() {
    printf("\n ***** LINEAS DEL ARCHIVO ***** \n");
    char *mem = MC_ptr;

    for (int i = 0; i < infoBasica->cantLineas; i++) {
        printf("[%i] %s\n", i, mem);
        mem += TAM_LINEA;
    }
}

void printProcesos(struct HiloProceso* procesos, int cantidad, char tipo, char *rw, int debeImprimir) {
    int cant = 0;
    for (int i = 0; i < cantidad; i++) {
        if (procesos[i].estado == tipo) {
            cant++; printf("[PID %lu]: %s\n", procesos[i].pid, rw);
        }
    }
    if (!cant && debeImprimir) printf("%s\n", "(No hay)");
}

void printEscritores() {
    printf("\n ***** ESTADO DE ESCRITORES ***** \n");
    struct HiloProceso *procesosEscritores = MC_Ctl_ptr + sizeof(struct InfoBasica) + sizeof(struct HiloProceso) * 100;

    printf("\n + Escribiendo + \n");
    printProcesos(procesosEscritores, infoBasica->cantEscritores, 'E', "Escritor", 1);
    printf("\n + Durmiendo + \n");
    printProcesos(procesosEscritores, infoBasica->cantEscritores, 'D', "Escritor", 1);
    printf("\n + Bloqueados + \n");
    printProcesos(procesosEscritores, infoBasica->cantEscritores, 'B', "Escritor", 1);
}

void printLectores() {
    printf("\n ***** ESTADO DE LECTORES ***** \n");
    struct HiloProceso *procesosLectores = MC_Ctl_ptr + sizeof(struct InfoBasica);

    printf("\n + Leyendo + \n");
    printProcesos(procesosLectores, infoBasica->cantLectores, 'L', "Lector", 1);
    printf("\n + Durmiendo + \n");
    printProcesos(procesosLectores, infoBasica->cantLectores, 'D', "Lector", 1);
    printf("\n + Bloqueados + \n");
    printProcesos(procesosLectores, infoBasica->cantLectores, 'B', "Lector", 1);
}

void printEgoistas() {
    printf("\n ***** ESTADO DE LECTORES EGOISTAS ***** \n");
    struct HiloProceso *procesosEgoistas = MC_Ctl_ptr + sizeof(struct InfoBasica) + sizeof(struct HiloProceso) * 200;

    printf("\n + Leyendo y Borrando + \n");
    printProcesos(procesosEgoistas, infoBasica->cantEgoistas, 'X', "Lector Egoista", 1);
    printf("\n + Durmiendo + \n");
    printProcesos(procesosEgoistas, infoBasica->cantEgoistas, 'D', "Lector Egoista", 1);
    printf("\n + Bloqueados + \n");
    printProcesos(procesosEgoistas, infoBasica->cantEgoistas, 'B', "Lector Egoista", 1);

}

void printTodos() {
    printf("\n ***** ESTADO DE TODOS LOS PROCESOS ***** \n");
    struct HiloProceso *procesosEgoistas = MC_Ctl_ptr + sizeof(struct InfoBasica) + sizeof(struct HiloProceso) * 200;
    struct HiloProceso *procesosLectores = MC_Ctl_ptr + sizeof(struct InfoBasica);
    struct HiloProceso *procesosEscritores = MC_Ctl_ptr + sizeof(struct InfoBasica) + sizeof(struct HiloProceso) * 100;

    printf("\n + Procesando el archivo + \n");
    printProcesos(procesosEscritores, infoBasica->cantEscritores, 'E', "Escritor", 0);
    printProcesos(procesosLectores, infoBasica->cantLectores, 'L', "Lector", 0);
    printProcesos(procesosEgoistas, infoBasica->cantEgoistas, 'X', "Lector Egoista", 0);

    printf("\n + Durmiendo + \n");
    printProcesos(procesosEscritores, infoBasica->cantEscritores, 'D', "Escritor", 0);
    printProcesos(procesosLectores, infoBasica->cantLectores, 'D', "Lector", 0);
    printProcesos(procesosEgoistas, infoBasica->cantEgoistas, 'D', "Lector Egoista", 0);

    printf("\n + Bloqueados + \n");
    printProcesos(procesosEscritores, infoBasica->cantEscritores, 'B', "Escritor", 0);
    printProcesos(procesosLectores, infoBasica->cantLectores, 'B', "Lector", 0);
    printProcesos(procesosEgoistas, infoBasica->cantEgoistas, 'B', "Lector Egoista", 0);
}

void menuInfo() {
    printf("\n\n!Bienvenido al Programa Espia del Archivo Compartido!\n\n");

    printf("*** Opciones para los Estados de Procesos y del Archivo ***\n");
    printf("Realice alguna de las siguientes opciones mediante la digitacion de su correspondiente numero\n");
    printf("1. Estado del Archivo (lineas)\n");
    printf("2. Estado de los Escritores\n");
    printf("3. Estado de los Lectores\n");
    printf("4. Estado de los Lectores Egoistas\n");
    printf("5. Estado de todos los procesos \n");
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

        if(!infoBasica->enJuego)
            break;
        if (opcionEstado <= 0 || opcionEstado >= 6) {
            printf("Opcion incorrecta. Intentelo de nuevo.\n");
            continue;
        }

        if (opcionEstado == 1)
            printLineasArchivo();
        else if (opcionEstado == 2)
            printEscritores();
        else if (opcionEstado == 3)
            printLectores();
        else if (opcionEstado == 4)
            printEgoistas();
        else
            printTodos();
    }

    printf("\nSe ha terminado la simulacion con el Terminador por lo que se ha terminado el proceso espia.\nPuede consultar la bitacora de ejecucion en el siguiente archivo \"%s\"\n",ARCHIVO_BITACORA);


    return 0;
}


