#include "pagina.h"

frame tab[NUMERO_FRAMES];
short int ocupacao_atual = 0;
short int page_fault = 0;
short int subs = 0;

void limpaFrame(short int i){
    tab[i].livre = true;
    tab[i].pagina = -1;
    tab[i].processo = -1;
    tab[i].tempo = -1;
}

void preencheFrame(short int i){
    struct timeval t;

    tab[i].livre = false;
    tab[i].pagina = i;
    tab[i].processo = getpid();

    gettimeofday(&t, NULL);
    tab[i].tempo = t.tv_sec * 1000000L + t.tv_usec;
}

void inicializaTab(){
    for(int i = 0; i < NUMERO_FRAMES; i++)
        limpaFrame(i);
}

void aloca(short int i){
    int j;

    if(ocupacao_atual < NUMERO_FRAMES){
        do{
            j = rand()%NUMERO_FRAMES;
        }while(!tab[j].livre);

        preencheFrame(j);
        page_fault++;
        ocupacao_atual++;
    }
    else{
        substitui(i);
    }
}

void substitui(short int i){

}

void shutdown(int proc){
    for(int i = 0; i <= proc; i++)
        cout << "Número de page faults do processo " << proc << ": " << page_fault << endl;
    cout << "Número de page faults total: " << page_fault << endl;
    cout << "Número de execuções do processo de substituição: " << subs << endl;
    cout << "Configuração final da memória:" << endl;

    cout << "Pagina \t Tempo de referencia" << endl;
    for(int i = 0; i < NUMERO_FRAMES; i++){
        if(tab[i].livre)
            cout << "Livre" << endl;
        else
            cout << tab[i].pagina << "\t" << tab[i].tempo << endl;
    }
}

void referencia_pagina(short int i){
    aloca(i);
}
