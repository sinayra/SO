#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <map>
#include "semaforo.h"
#include "fila_mensagem.h"
//Includes de chamadas de sistema
#include <sys/time.h>
#include <sys/shm.h>
#include <unistd.h>

#define MAX_OCUPACAO 9
#define OCUPACAO_OK 8
#define NUMERO_FRAMES 10

using namespace std;
///Frame
//linha da tabela de páginas

typedef enum{
    LIVRE,
    OCUPADO,
    RESERVADO
}tipoEstado;

typedef struct{
    int pagina;
    long int tempo;
    int processo;
    int pid;
    tipoEstado estado;
}frame;

typedef struct{
    long int tempo;
    int index;
}frameAux;

///Inicializa tab com frames livres
void inicializaTab();

///Alocação de Páginas
//reserva um pageframe para página enviada por mensagem
//Se página não estiver alocada, escolhe aleatoriamente frames livres e ocorre page fault
void aloca();

///Substituição de Páginas
//Caso não há frames livres, executa LRU
void substitui();

///Terminar execução dos processos
// Imprime na tela
//  a) Número de page faults de cada processo
//  b) Número total de page faults
//  c) Número de substituições
//  d) Configuração final da tabela de páginas
void shutdown();

///Reserva uma page frame para página i
void referencia_pagina(int i, int processo);
