/*
 ============================================================================
 Name        : Writer.c
 Author      : 
 Version     : 0.0.1
 Copyright   : Your copyright notice
 Description : writer in C, Ansi-style
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <pthread.h>

#include "Cola.h"
#include "Semaforo.h"
#include "CrearEscritores.h"
#include "Hilo.h"


int main(int argc, int *argv[]) {
	//datos iniciales
	int final=1;
	int * pfinal=&final;
	//cantidad de procesos
	//int cantidad = strtol(argv[1], NULL, 10);
	int cantidad = 5;

	//cola
	Cola * cola = (Cola*)malloc(sizeof(Cola));

	//semaforo
	sem_t *psem_1=sem_open("SEM", 0);

	//memoria compartida
	int largo_3 = sizeof(int )*2;
	key_t shmkey_3=0x1234563;
	int shmid_3;
    int * largo;
	shmid_3 = shmget (shmkey_3, largo_3, 0644 | IPC_CREAT);
    if (shmid_3 < 0){
        perror ("shmget\n");
        exit (1);
    }
    largo = (int *)shmat(shmid_3, 0, 0);


	int largo_2 = largo[1];
	key_t shmkey_2=0x1234562;
	int shmid_2;
    int * estado;
	shmid_2 = shmget (shmkey_2, largo_2, 0644 | IPC_CREAT);
    if (shmid_2 < 0){
        perror ("shmget\n");
        exit (1);
    }
    estado = (int *)shmat(shmid_2, 0, 0);

	int largo_1 = largo[0];
	key_t shmkey_1=0x1234561;
	int shmid_1;
    int *sms;

    shmid_1 = shmget (shmkey_1, largo_1, 0644 | IPC_CREAT);
	if (shmid_1 < 0){
		perror ("shmget\n");
		exit (1);
	}
	sms = (int *)shmat(shmid_1, 0, 0);

    //crear procesos
    sem_wait(psem_1);
	int lineas=estado[1];
	int largo_linea=estado[2];
	int writers=cantidad;
	int writers_max=estado[4];
	int writers_init=estado[7];
	sem_post(psem_1);
    crear_escritor(cola,cantidad,estado,pfinal,sms,psem_1,
    		lineas,largo_linea,writers,writers_max,writers_init);


    Nodo * actual;
    /*

    actual=cola->inicio;
    while(actual!=NULL){
        iniciar_hilo(actual);
    	actual=actual->siguiente;
    }
    printf("Test 0 \n");
    cola->writers=cantidad;
    cola->estado=estado;
    */
    cola->finalizar=pfinal;
    cola->semaforo_1=psem_1;
    printf("Test 1 \n");
    iniciar_hilo_estado(cola);
    printf("Test 2 \n");
    /*

    actual=cola->inicio;
    while(actual!=NULL){
    	ejecutar_hilo(actual);
    	actual=actual->siguiente;

    }
    */
    printf("Test 4 \n");
    ejecutar_mostrar_estado(cola);

    printf("Test 3 \n");

	return EXIT_SUCCESS;
}
