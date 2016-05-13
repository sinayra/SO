#pragma once
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/errno.h>
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

int inicializaMsg();

mensagem enviaMsg(int id, mensagem msg);
mensagem recebeMsg(int id);

void destroiMsg(int id);
