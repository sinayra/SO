#include "pagina.h"

//globais do processo atual
int proc;
int proc_page_fault = 0;
//Memória compartilhada
int *subs, *page_faults, *ocupacao_atual;
frame *tab;

void limpaFrame(int i){

    tab[i].livre = true;
    tab[i].pagina = -1;
    tab[i].processo = -1;
    tab[i].tempo = -1;
    tab[i].pid = -1;
}

void preencheFrame(int i, int pagina){
    struct timeval t;

    tab[i].livre = false;
    tab[i].pagina = pagina;
    tab[i].processo = proc;

    gettimeofday(&t, NULL);
    tab[i].tempo = t.tv_sec * 1000000L + t.tv_usec;

}

bool ordenaFrame(frameAux v1, frameAux v2){
    return v1.tempo < v2.tempo;
}

int getPagina(int i){

    for(int j = 0; j < NUMERO_FRAMES; j++){
        if(tab[j].processo == proc && tab[j].pagina == i)
            return j;
    }
    return -1;
}

void defineProcesso(int processo, int ids[4]){
    proc = processo;

    tab = (frame *)shmat(ids[3], 0, 0);
    if(tab == (frame*)-1){
        cout << "Erro em shmat() para tab" << endl;
        exit(EXIT_FAILURE);
    }

    subs = (int *)shmat(ids[0], 0, 0);
    if(subs == (int*)-1){
        cout << "Erro em shmat() para subs" << endl;
        exit(EXIT_FAILURE);
    }

    page_faults = (int *)shmat(ids[1], 0, 0);
    if(page_faults == (int*)-1){
        cout << "Erro em shmat() para page_faults" << endl;
        exit(EXIT_FAILURE);
    }

    ocupacao_atual = (int *)shmat(ids[2], 0, 0);
    if(ocupacao_atual == (int*)-1){
        cout << "Erro em shmat() para ocupacao_atual" << endl;
        exit(EXIT_FAILURE);
    }
}

void inicializaTab(char *path, int ids[4]){

    ids[0] = shmget(IPC_PRIVATE, sizeof(int), 0666 | IPC_CREAT);
    ids[1] = shmget(IPC_PRIVATE, sizeof(int), 0666 | IPC_CREAT);
    ids[2] = shmget(IPC_PRIVATE, sizeof(int), 0666 | IPC_CREAT);
    ids[3] = shmget(IPC_PRIVATE, sizeof(frame) * NUMERO_FRAMES, 0666 | IPC_CREAT);

    if(ids[0] == -1 || ids[1] == -1 || ids[2] == -1 || ids[3] == -1){
        cout << "Erro com shmget()" << endl;
        exit(EXIT_FAILURE);
    }

    defineProcesso(-1, ids); //esse e o pai

    for(int i = 0; i < NUMERO_FRAMES; i++)
        limpaFrame(i);
}

void aloca(int i){
    int j;
    int pag = getPagina(i);

    if(pag < 0){
        if(*ocupacao_atual > MAX_OCUPACAO){
            substitui(i);
        }
        else{
            do{
                j = rand()%NUMERO_FRAMES;
            }while(!tab[j].livre);
            preencheFrame(j, i);
            (*page_faults)++;
            (*ocupacao_atual)++;
            proc_page_fault++;
        }
    }
    else{
        preencheFrame(pag, i);
    }
}

void substitui(int i){
    int j;
    vector<frameAux> v(NUMERO_FRAMES);

    (*subs)++;

    for(int i = 0; i < NUMERO_FRAMES; i++){
        v[i] = {tab[i].tempo, i};
    }

    sort(v.begin(), v.end(), ordenaFrame);

    j = 0;
    while(*ocupacao_atual >= OCUPACAO_OK){
        limpaFrame(v[j].index);
        j++;
        (*ocupacao_atual)--;
    }

    do{
        j = rand()%NUMERO_FRAMES;
    }while(!tab[j].livre);

    preencheFrame(j, i);
    proc_page_fault++;
    (*page_faults)++;
    (*ocupacao_atual)++;
}

void shutdownFinal(){

    cout << "Número de page faults total: " << *page_faults << endl;
    cout << "Número de execuções do processo de substituição: " << *subs << endl;
    cout << "Configuração final da memória:" << endl;

    cout << "Processo\t Pagina \t Tempo de referencia" << endl;
    for(int i = 0; i < NUMERO_FRAMES; i++){
        if(tab[i].livre)
            cout << "Livre" << endl;
        else
            cout << tab[i].processo << "\t\t\t" << tab[i].pagina << "\t" << tab[i].tempo << endl;
    }
}

void shutdown(int pid){

    if(pid == 0)
        cout << "Número de page faults do processo " << proc << ": " << proc_page_fault << endl;
    else
        shutdownFinal();

    shmdt(ocupacao_atual);
    shmdt(page_faults);
    shmdt(subs);
    shmdt(tab);
}

void referencia_pagina(int i){
    aloca(i);
}
