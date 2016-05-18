#pragma once

#include <vector>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <unistd.h>

#include <iostream>
using namespace std;



///Retorna indice do novo semaforo
int inicializaSem();

void P(int idsem);

void V(int idsem);

void destroiSem(int idsem);
