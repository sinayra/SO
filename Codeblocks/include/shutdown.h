#pragma once

#include <iostream>

#include "fila_mensagem.h"
#include "memoria_compartilhada.h"
#include "semaforo.h"
#include "utils.h"
#include <map>

using namespace std;

///Terminar execução dos processos
// Imprime na tela
//  a) Número de page faults de cada processo
//  b) Número total de page faults
//  c) Número de substituições
//  d) Configuração final da tabela de páginas
void shutdown();
