#include "../include/alocador.h"

extern int req, resp;
map<int, int> proc;

extern void encerra(int signo);
extern int sem, proc_atual;
extern int *ocupacao_atual;
extern frame *tab;

void alocador(){
    mensagem msg_r, msg_s;

    signal(SIGUSR1, encerra);

    while(1){
        msg_r = recebeMsg(req);

        referencia_pagina(msg_r.pagina, msg_r.processo);

        msg_s = msg_r;
        msg_s.alocado = true;
        enviaMsg(resp, msg_s, 2);

    }
}

void aloca(int i){
    int j, index;

    P(sem);
    index = getPagina(i);

    if(index < 0){

            if(*ocupacao_atual >= NUMERO_FRAMES)
                substitui();
            do{
                j = rand()%NUMERO_FRAMES;
            }while(tab[j].estado != LIVRE);

            preencheFrame(j, i);

            (*ocupacao_atual)++;

            proc[proc_atual]++;

    }
    else
            preencheFrame(index, i);
    V(sem);
}

void referencia_pagina(int i, int processo){

    map<int, int>::iterator it = proc.begin();
    proc_atual = processo;

    proc.insert(it, pair<int, int>(processo, 0)); //se ja tiver esse processo, nao insere

    aloca(i);
}
