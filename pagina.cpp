#include "pagina.h"

//Memória compartilhada
frame tab[NUMERO_FRAMES];
int ocupacao_atual = 0;
int page_fault = 0;
int subs = 0;

//globais do processo atual
int proc;
int proc_page_fault = 0;
int *mc_subs, *mc_page_faults, *mc_ocupacao_atual;
frame *mc_tab;

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
    tab[i].pid = getpid();

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

void defineProcesso(int processo){
    proc = processo;
}

void inicializaTab(char *path){
    key_t k;
    int id_subs, id_page_fault, id_ocupacao_total, id_tab;

    k = ftok(path, 'P');
    if(k < 0){
        cout << "Erro com ftok()" << endl;
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < NUMERO_FRAMES; i++)
        limpaFrame(i);
}

void aloca(int i){
    int j;
    int pag = getPagina(i);

    if(pag < 0){
        if(ocupacao_atual > MAX_OCUPACAO){
            substitui(i);
        }
        else{
            do{
                j = rand()%NUMERO_FRAMES;
            }while(!tab[j].livre);

            preencheFrame(j, i);
            proc_page_fault++;
            page_fault++;
            ocupacao_atual++;
        }
    }
    else{
        preencheFrame(pag, i);
    }
}

void substitui(int i){
    int j;
    vector<frameAux> v(NUMERO_FRAMES);

    subs++;

    for(int i = 0; i < NUMERO_FRAMES; i++)
        v[i] = {tab[i].tempo, i};

    sort(v.begin(), v.end(), ordenaFrame);

    j = 0;
    while(ocupacao_atual >= OCUPACAO_OK){
        limpaFrame(v[j].index);
        j++;
        ocupacao_atual--;
    }

    do{
        j = rand()%NUMERO_FRAMES;
    }while(!tab[j].livre);

    preencheFrame(j, i);
    proc_page_fault++;
    page_fault++;
    ocupacao_atual++;
}

void shutdownFinal(){
    cout << "Número de page faults total: " << page_fault << endl;
    cout << "Número de execuções do processo de substituição: " << subs << endl;
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
}

void referencia_pagina(int i){
    aloca(i);
}
