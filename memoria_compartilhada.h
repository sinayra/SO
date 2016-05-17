#pragma once

#include "utils.h"
#include <sys/shm.h>

int inicializaMemInt(int num = 1);
int inicializaMemFrame(int num = 1);

void associaMemFrame(frame **mem, int id);
void associaMemInt(int **mem, int id);

void destroiMem(void *mem);
