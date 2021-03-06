#include "../util.c"

struct InfoBasica* infoBasica;
struct HiloProceso* procesosEgoistas;

int durDormir, durLeerEgoista, numEgoistas, contadorEgoistas=0;
char * MC_ptr;

sem_t semLecturaEgoista;

void leerEgoista(int i) {
    procesosEgoistas[i].pid = pthread_self();

    char *mem, *linea = malloc(TAM_LINEA);
    int numLinea = 0, numLineaResp, valorPrimerLector, contadorNoEgoistas;

    do {
        procesosEgoistas[i].estado = 'B';
        sem_wait(&semLecturaEgoista);

        contadorEgoistas++;
        if(contadorEgoistas==3) {
            sem_wait(&infoBasica->semEgoista);
        }
        if(contadorEgoistas>3 ) {
            sem_wait(&infoBasica->semEgoista);
            sem_wait(&infoBasica->semAcumuladoEgoistas); //W
            if(infoBasica->acumuladoEgoistas==0)
               contadorEgoistas=1;
            sem_post(&infoBasica->semEgoista);
            sem_post(&infoBasica->semAcumuladoEgoistas);//P
        }

        sem_post(&semLecturaEgoista);
        sem_wait(&infoBasica->semContadorModificacion);
        sem_wait(&infoBasica->semControl);

        //region critica
        sem_post(&infoBasica->semContadorModificacion);
        if(infoBasica->primerLectorTermino) {
            infoBasica->primerLectorTermino = FALSE;
            sem_post(&infoBasica->semPrimerLector);
        }
        numLineaResp = numLinea;
        procesosEgoistas[i].estado = 'X';

        int lineasLlenas[infoBasica->cantLineas], cantLineasLlenas = 0;
        do {
            mem = MC_ptr + (TAM_LINEA * numLinea);
            memcpy(linea,mem , TAM_LINEA);

            if (strcmp(linea, "")) {
                lineasLlenas[cantLineasLlenas++] = numLinea;
            }
            numLinea = (numLinea + 1) % infoBasica->cantLineas;
        } while (numLinea != numLineaResp);

        char *mensajeBitacora = malloc(1000);

        if (!cantLineasLlenas) {
            snprintf(mensajeBitacora, 1000, "[%s] PID: %lu (Lector Egoista) - No lee porque el archivo esta vacio\n", getTime(), pthread_self());
        } else {

            int lineaRandom = rand() % cantLineasLlenas;
            mem = MC_ptr + (TAM_LINEA * lineasLlenas[lineaRandom]);
            memcpy(linea,mem , TAM_LINEA);

            sleep(durLeerEgoista);
            snprintf(mensajeBitacora, 1000, "[%s] PID: %lu (Lector Egoista) - Lee y elimina \"%s\"\n", getTime(), pthread_self(), linea);

            for (int s = 0; s < TAM_LINEA; s++) *mem++ = '\0';
        }
        printf("%s", mensajeBitacora);

        FILE * fp = fopen (ARCHIVO_BITACORA,"a");
        fprintf(fp, "%s", mensajeBitacora);
        fclose (fp);

        sem_wait(&infoBasica->semAcumuladoEgoistas); //W
        infoBasica->acumuladoEgoistas++;
        sem_post(&infoBasica->semAcumuladoEgoistas);//P
        sem_post(&infoBasica->semControl); //

        sem_wait(&infoBasica->semAcumuladoEgoistas); //W
        sem_getvalue(&infoBasica->semContadorNoEgoistas,&contadorNoEgoistas);
        if(infoBasica->acumuladoEgoistas >= 3 && contadorNoEgoistas==101)
            sem_post(&infoBasica->semEgoista);
        sem_post(&infoBasica->semAcumuladoEgoistas);//P

        procesosEgoistas[i].estado = 'D';
        sleep(durDormir);

        numLinea = (numLinea + 1) % infoBasica->cantLineas;
    } while (TRUE);
}

int main() {
    int MC_Ctl_Id;
    FILE * fp = fopen (ARCHIVO_DE_CONTROL,"r");
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

    printf( "Ingrese la cantidad de lectores egoistas deseados: ");
    scanf("%d",  &numEgoistas);
    printf( "Ingrese la cantidad  de segundos para dormir: ");
    scanf("%d",  &durDormir);
    printf( "Ingrese la cantidad  de segundos para leer: ");
    scanf("%d",  &durLeerEgoista);

    infoBasica->cantEgoistas = numEgoistas;
    procesosEgoistas = MC_Ctl_ptr + sizeof(struct InfoBasica) + sizeof(struct HiloProceso) * 200;

    sem_init(&semLecturaEgoista,1,1);

    pthread_t hilosEgoistas[numEgoistas];
    for (int i = 0; i < numEgoistas; i++) {
        pthread_create(&hilosEgoistas[i], NULL, leerEgoista, i);
    }
    while(infoBasica->enJuego)
        sleep(1);

    for (int i = 0; i < numEgoistas; i++) {
        pthread_cancel(hilosEgoistas[i]);
    }

    printf("\nSe ha terminado la simulacion con el Terminador por lo que se han terminado los hilos de procesos lectores egoistas y con ello tambien terminara este proceso\n");

}