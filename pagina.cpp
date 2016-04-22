#include "pagina.h"

vector<frame> tab(NUMERO_FRAMES);
short int ocupacao_atual = 0;
short int page_fault = 0;
short int subs = 0;
int proc;

void limpaFrame(short int i){
    tab[i].livre = true;
    tab[i].pagina = -1;
    tab[i].processo = -1;
    tab[i].tempo = -1;
}

void preencheFrame(short int i, int pagina){
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

int getPagina(short int i){
    for(int j = 0; j < NUMERO_FRAMES; j++){
        if(tab[j].pagina == i && tab[j].processo == proc)
            return j;
    }
    return -1;
}

void inicializaTab(int processo){
    for(int i = 0; i < NUMERO_FRAMES; i++)
        limpaFrame(i);

    proc = processo;
}

void aloca(short int i){
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
            page_fault++;
            ocupacao_atual++;
        }
    }
    else{
        preencheFrame(pag, i);
    }
}

void substitui(short int i){
    int j;
    vector<frameAux> v(NUMERO_FRAMES);

    subs++;

    for(short int i = 0; i < NUMERO_FRAMES; i++)
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
    page_fault++;
    ocupacao_atual++;
}

void shutdown(){
    for(int i = 0; i <= proc; i++)
        cout << "Número de page faults do processo " << proc << ": " << page_fault << endl;
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

void referencia_pagina(short int i){
    aloca(i);
}
