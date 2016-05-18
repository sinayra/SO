#pragma once
#include "utils.h"
#include "fila_mensagem.h"

#include <fstream>
#include <sstream>

#include <unistd.h>

///Processos de usuario
//Manda requisicao para o alocador para alocar uma pagina de seu processo
//A cada requisicao, recebe uma resposta do alocador indicando se alocacao foi bem sucessida
//Termina execucao quando todas suas paginas forem referenciadas
void processo(int proc);
