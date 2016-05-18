#pragma once

#include <signal.h>
#include <map>
#include "pagina.h"
#include "substituidor.h"
#include "utils.h"

///Alocador de Paginas
//Recebe uma requisicao de alocacao de pagina de um dado processo e coloca esta requisicao em uma fila
//Ao terminar processo de alocacao, manda mensagem ao processo que alocacao teve sucesso
void alocador();

///Reserva uma page frame para pagina i
//Sincroniza uma alocacao de pagina por vez e inicializa contador de page_fault do processo
void referencia_pagina(int i, int processo);

///Alocacao
//Se nao houver paginas livres, executa substitui(), de substituidor
//Se pagina nao estiver alocada, escolhe aleatoriamente um frame livre e ocorre page fault
//Se pagina estiver alocada, atualiza tempo de referenciamento
void aloca(int i);
