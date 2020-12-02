#include "../util.c"

int contadorLectores = 0, primerLector;
sem_t semLectura;

struct InfoBasica* infoBasica;
struct HiloProceso* procesosLectores;

void *thread_function(void *);

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

int durDormir, durLeer, numLectores;
char * MC_ptr;

_Noreturn void leer(int i) {
    procesosLectores[i].pid = pthread_self();

    char *mem, *linea = malloc(TAM_LINEA);
    int numLinea = 0, numLineaResp, contadorModificadores;

    do {
        sem_getvalue(&infoBasica->semContadorModificacion,&contadorModificadores);
        if( contadorModificadores!=200 && infoBasica->primerLectorTermino) {
            sem_wait(&infoBasica->semPrimerLector);
            sem_post(&infoBasica->semPrimerLector);
        }
        procesosLectores[i].estado = 'B';
        sem_wait(&semLectura);
        contadorLectores++;
        if(contadorLectores == 1) {
            primerLector = i;
            sem_wait(&infoBasica->semContadorNoEgoistas);
            sem_wait(&infoBasica->semControl);
            sem_post(&infoBasica->semContadorNoEgoistas);
        }
        sem_post(&semLectura);

        //region critica
        sem_wait(&infoBasica->semAcumuladoEgoistas); //W

        if(infoBasica->acumuladoEgoistas >= 3) {
            sem_post(&infoBasica->semEgoista);
        }
        infoBasica->acumuladoEgoistas=0;
        sem_post(&infoBasica->semAcumuladoEgoistas);//P
        numLineaResp = numLinea;
        procesosLectores[i].estado = 'L';

        do {
            mem = MC_ptr + (TAM_LINEA * numLinea);
            memcpy(linea,mem , TAM_LINEA);

            if (strcmp(linea, ""))
                break;
            numLinea = (numLinea + 1) % infoBasica->cantLineas;
        } while (numLinea != numLineaResp);

        char* mensajeBitacora = malloc(1000);

        if (numLinea == numLineaResp && !strcmp(linea, ""))
            snprintf(mensajeBitacora, 1000, "[%s] PID: %lu (Lector) - No lee porque el archivo esta vacio\n", getTime(), pthread_self());
        else {
            sleep(durLeer);
            snprintf(mensajeBitacora, 1000, "[%s] PID: %lu (Lector) - Lee \"%s\"\n", getTime(), pthread_self(), linea);
        }
        printf("%s", mensajeBitacora);
        // ESCBE EN BITACORA

        sem_wait(&semLectura);

        FILE * fp = fopen (ARCHIVO_BITACORA,"a");
        fprintf(fp, "%s", mensajeBitacora);
        fclose (fp);

        contadorLectores--;
        if(primerLector == i && !infoBasica->primerLectorTermino) {
            infoBasica->primerLectorTermino = TRUE;
            sem_wait(&infoBasica->semPrimerLector);
        }
        if(contadorLectores==0)
            sem_post(&infoBasica->semControl);
        sem_post(&semLectura);

        procesosLectores[i].estado = 'D';
        sleep(durDormir);

        numLinea = (numLinea + 1) % infoBasica->cantLineas;
    } while (TRUE);
}

int main() {
    int MC_Ctl_Id;
    FILE * fp = fopen (ARCHIVO_DE_CONTROL,"r");
    fscanf(fp, "%i", &MC_Ctl_Id);
    fclose (fp);

    sem_init(&semLectura,1,1);

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

    printf( "Ingrese la cantidad de lectores deseados: ");
    scanf("%d",  &numLectores);
    printf( "Ingrese la cantidad  de segundos para dormir: ");
    scanf("%d",  &durDormir);
    printf( "Ingrese la cantidad  de segundos para leer: ");
    scanf("%d",  &durLeer);

    infoBasica->cantLectores = numLectores;
    procesosLectores = MC_Ctl_ptr + sizeof(struct InfoBasica);

    pthread_t hilosLectores[numLectores];
    for (int i = 0; i < numLectores; i++) {
        pthread_create(&hilosLectores[i], NULL, leer, i);
    }
    while(infoBasica->enJuego)
        sleep(1);

    for (int i = 0; i < numLectores; i++) {
        pthread_cancel(hilosLectores[i]);
    }

    printf("\nSe ha terminado la simulacion con el Terminador por lo que se han terminado los hilos de procesos lectores y con ello tambien terminara este proceso\n");

}
