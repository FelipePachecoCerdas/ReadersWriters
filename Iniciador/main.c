#include "../util.c"


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

    printf( "\nLa ID de la memoria compartida (archivo) es: %d\n", MC_Id);

    char * MC_ptr = shmat(MC_Id, NULL, 0);
    for (int i = 0; i < TAM_LINEA * cantLineas; i++) {
        MC_ptr[i] = '\0';
    }

    char * mem = MC_ptr+100;
    /*for (int i = 1; i < cantLineas; i+=2) {
        char * linea = malloc(100);
        snprintf(linea, 100, "Hola soy %i", i);

        memcpy(mem, linea,  strlen(linea));
        mem += TAM_LINEA*2;
    }*/

    key_t MC_Ctl_Llave = ftok(".", 'HOLA');
    int MC_Ctl_Id = shmget(MC_Ctl_Llave, sizeof(struct InfoBasica) + sizeof(struct HiloProceso) * 300, IPC_CREAT | 0666);
    char * MC_Ctl_ptr = shmat(MC_Ctl_Id, NULL, 0);

    struct InfoBasica* infoBasica = (struct InfoBasica*) MC_Ctl_ptr;
    infoBasica->MC_Id = MC_Id;
    infoBasica->cantLineas = cantLineas;
    infoBasica->acumuladoEgoistas = 0;
    infoBasica->enJuego = 1;
    sem_init(&infoBasica->semControl, 1, 1);
    sem_init(&infoBasica->semEgoista, 1, 1);
    sem_init(&infoBasica->semPrimerLector, 1, 1);
    sem_init(&infoBasica->semContadorModificacion,1,200);
    sem_init(&infoBasica->semContadorNoEgoistas,1,101);
    infoBasica->primerLectorTermino=FALSE;
    infoBasica->cantEgoistas = infoBasica->cantEscritores = infoBasica->cantLectores = 0;

    FILE * fp = fopen (ARCHIVO_DE_CONTROL,"w");
    fprintf(fp, "%i", MC_Ctl_Id);
    fclose (fp);

    FILE * fp1 = fopen (ARCHIVO_BITACORA,"w");
    fprintf(fp1, "%s", "");
    fclose (fp1);
    printf("\nLa ID de la memoria compartida de control es: %d, la puede consultar en el archivo \"%s\" \n", MC_Ctl_Id, ARCHIVO_DE_CONTROL);

    printf("\nEste proceso ha terminado\n");

    return 0;
}


