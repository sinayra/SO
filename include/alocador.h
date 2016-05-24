#pragma once

#include <signal.h>
#include <map>
#include "pagina.h"
#include "substituidor.h"
#include "utils.h"

///Alocador de Paginas
//Recebe uma requisicao de alocacao de pagina de um dado processo e coloca esta requisicao em uma fila
//Se o processo nao tiver contador de page faults, inicializa o contador
//Ao terminar processo de alocacao, manda mensagem ao processo que alocacao teve sucesso
void alocador();


///Reserva uma page frame para pagina i
//Se nao houver paginas livres, executa substitui(), de substituidor
//Se pagina nao estiver alocada, escolhe aleatoriamente um frame livre e ocorre page fault
//Se pagina estiver alocada, atualiza tempo de referenciamento
void aloca(int i);
