#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
//Includes de chamadas de sistema
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

#define MAX_OCUPACAO 9
#define OCUPACAO_OK 8
#define NUMERO_FRAMES 10

using namespace std;
///Frame
//linha da tabela de páginas
typedef struct{
    short int pagina;
    long int tempo;
    short int processo;
    bool livre;
}frame;

typedef struct{
    long int tempo;
    short int index;
}frameAux;

///Inicializa tab com frames livres
void inicializaTab(int processo);

///Alocação de Páginas
//reserva um pageframe para página i
//Se página não estiver alocada, escolhe aleatoriamente frames livres e ocorre page fault
void aloca(short int i);

///Substituição de Páginas
//Caso não há frames livres, executa LRU
void substitui(short int i);

///Terminar execução dos processos
// Imprime na tela
//  a) Número de page faults de cada processo
//  b) Número total de page faults
//  c) Número de substituições
//  d) Configuração final da tabela de páginas
void shutdown();

///Reserva uma page frame para página i
void referencia_pagina(short int i);
