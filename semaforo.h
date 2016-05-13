#pragma once
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

int inicializaSem();

void P(int idsem);

void V(int idsem);

void finalizaSem(int idsem);
