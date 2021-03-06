/*
 ============================================================================
 Name        : Star.c
 Author      : 
 Version     : 0.0.1
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
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

int main(int argc, int *argv[]) {

	int lineas = 20;
	if(argc>1){
		lineas = strtol(argv[1], NULL, 10);
	}

	int largo_linea = 10;
	int tamano = lineas * largo_linea;

	//memoria compartidad
	int largo_1 = sizeof(int )*tamano;
	key_t shmkey_1=0x1234561;
	int shmid_1;
	int *sms;
    shmid_1 = shmget (shmkey_1, largo_1, 0664 | IPC_CREAT);
    if (shmid_1 < 0){
        perror ("shmget\n");
        exit (1);
    }
    sms = (int *)shmat(shmid_1, NULL, 0);
    for(int i=0;i<lineas;i++){
		sms[i*largo_linea]=i;
    }

    //estados
    int informacion = 14;
    int writers = 20;
    int readers = 20;
    int readers_selfish = 20;
    int writers_i = writers*2;
	int readers_i = readers*3;
	int readers_selfish_i = readers_selfish*3;
    int writers_init = informacion;
    int readers_init = writers_init+writers_i;
    int readers_selfish_init = readers_init+readers_i;
    int total = readers_selfish_init+readers_selfish_i;

    //trasmicion de estados
    int largo_2 = sizeof(int)*total;
	key_t shmkey_2=0x1234562;
	int shmid_2;
	int * estado;
	shmid_2 = shmget (shmkey_2, largo_2, 0664 | IPC_CREAT);
	if (shmid_2 < 0){
		perror ("shmget\n");
		exit (1);
    }

	estado = (int *)shmat(shmid_2, 0, 0);

	estado[0] = 1;
	estado[1] = lineas;
	estado[2] = largo_linea;
	estado[3] = tamano;
	estado[4] = writers;
	estado[5] = readers;
	estado[6] = readers_selfish;
	estado[7] = writers_init;
	estado[8] = readers_init;
	estado[9] = readers_selfish_init;

	//tamaño
	int largo_3 = sizeof(int )*2;
	key_t shmkey_3=0x1234563;
	int shmid_3;
    shmid_3 = shmget (shmkey_3, largo_3, 0664 | IPC_CREAT);
    if (shmid_3 < 0){
        perror ("shmget\n");
        exit (1);
    }

	int * dato;
	dato = (int *)shmat(shmid_3, 0, 0);
	dato[0] = largo_1;
	dato[1] = largo_2;

	sem_t * sem_1 = sem_open("SEM_1",
            O_CREAT, S_IRUSR | S_IWUSR, 1);

	sem_t * sem_2 = sem_open("SEM_2",
            O_CREAT, S_IRUSR | S_IWUSR, 1);
	printf("\033[H\033[J");
	printf("*****************************************\n");
	printf("Inicializador \n");
	printf("*****************************************\n");

	exit (1);
}
