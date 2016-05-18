#pragma once

#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <climits>

#include "semaforo.h"
#include "fila_mensagem.h"
#include "memoria_compartilhada.h"
#include "utils.h"
//Includes de chamadas de sistema
#include <sys/time.h>
#include <unistd.h>

using namespace std;

///Inicializa vetor de frames e mecanismos de sincronizacao
void inicializaMemVirtual();

///Retorna indice da pagina do processo referenciado
//se nao existir, retorna -1
int getPagina(int i);

///Preenche frame i, deixando-o com status OCUPADO
void preencheFrame(int i, int pagina);

///Preenche frame i, deixando-o com status LIVRE
void limpaFrame(int i);

///QuickSort para ordenar tabela, do maior para menor
//Utiliza sort do c++
bool ordenaFrame(frameAux v1, frameAux v2);
