#pragma once

#include <signal.h>
#include "pagina.h"
#include "utils.h"

///Verifica periodicamente se pode executar sub-rotina substitui()
void substituidor();

///Substituição de Páginas
//Executa politica LRU
void substitui();
