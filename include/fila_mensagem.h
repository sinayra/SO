#pragma once
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>
#include <cstdlib>

using namespace std;

typedef struct{
    long mtype;
    int processo;
    bool alocado;
    int pagina;
    int page_faults;
}mensagem;

///Cria nova fila de mensagem
//retorna id desta fila
int inicializaMsg();

///Envia mensagem para uma fila id
void enviaMsg(int id, mensagem msg, long type = 1);

///Recebe mensagem da fila id
mensagem recebeMsg(int id, long type = 1);

///Destroi fila de mensagem id
void destroiMsg(int id);
