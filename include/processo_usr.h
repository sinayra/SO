#pragma once
#include "utils.h"
#include "fila_mensagem.h"

#include <fstream>
#include <sstream>

#include <unistd.h>

///Processos de usuario
//Abre arquivo e referencia pagina para cada pagina contida no arquivo
void processo(int proc);

///Referencia pagina
//Manda requisicao para o alocador para alocar uma pagina de seu processo
//A cada requisicao, recebe uma resposta do alocador indicando se alocacao foi bem sucessida
//Termina execucao quando todas suas paginas forem referenciadas
bool referencia_pagina(int i, int processo);
