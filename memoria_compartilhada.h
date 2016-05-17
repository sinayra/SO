#pragma once

#include "utils.h"
#include <sys/shm.h>

///Cria memoria compartilhada
int inicializaMemInt(int num = 1);
int inicializaMemFrame(int num = 1);

///Associa memoria compartilhada a uma variavel
void associaMemFrame(frame **mem, int id);
void associaMemInt(int **mem, int id);

///Dessassocia variavel a memoria compartilhada
void destroiMem(void *mem);
