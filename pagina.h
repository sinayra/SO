#pragma once

#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <map>

#include "semaforo.h"
#include "fila_mensagem.h"
#include "memoria_compartilhada.h"
#include "utils.h"
//Includes de chamadas de sistema
#include <sys/time.h>
#include <unistd.h>

using namespace std;

///Inicializa tab com frames livres
void inicializaTab();

///Alocação de Páginas
//reserva um pageframe para página enviada por mensagem
//Se página não estiver alocada, escolhe aleatoriamente frames livres e ocorre page fault
void aloca();

///Substituição de Páginas
//Caso não há frames livres, executa LRU
void substitui();

///Reserva uma page frame para página i
void referencia_pagina(int i, int processo);
