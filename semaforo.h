#pragma once

#include <vector>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <iostream>
using namespace std;

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};

///Retorna indice do novo semaforo
int inicializaSem();

void P(int idsem, int index);

void V(int idsem, int index);

void destroiSem(int idsem);
