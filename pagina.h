#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

#define MAX_OCUPACAO 9
#define NUMERO_FRAMES 10

using namespace std;
///Frame
//linha da tabela de páginas
typedef struct{
    short int pagina;
    time_t tempo;
    short int pagina;
}frame;

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
