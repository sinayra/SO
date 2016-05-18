#include "pagina.h"

//Globais do processo executando atualmente
int proc_atual = -1;
int msg, sem;
//Memória compartilhada
int *subs, *ocupacao_atual;
frame *tab;

void limpaFrame(int i){
    tab[i].estado = LIVRE;
    tab[i].pagina = -1;
    tab[i].processo = -1;
    tab[i].tempo = -1;
    tab[i].pid = -1;
}

void preencheFrame(int i, int pagina){
    struct timeval t;

    gettimeofday(&t, NULL);

    tab[i].estado = OCUPADO;
    tab[i].pagina = pagina;
    tab[i].processo = proc_atual;
    tab[i].tempo = t.tv_sec * 1000000L + t.tv_usec;

}

bool ordenaFrame(frameAux v1, frameAux v2){
    return v1.tempo < v2.tempo;
}

int getPagina(int i){

    for(int j = 0; j < NUMERO_FRAMES; j++){
        if(tab[j].pagina == i && tab[j].processo == proc_atual)
            return j;
    }
    return -1;
}

void inicializaTab(){
    int ids[3];
    ids[0] = inicializaMemInt();
    ids[1] = inicializaMemInt();
    ids[2] = inicializaMemFrame(NUMERO_FRAMES);

    associaMemInt(&subs, ids[0]);
    associaMemInt(&ocupacao_atual, ids[1]);
    associaMemFrame(&tab, ids[2]);

    sem = inicializaSem();

    *ocupacao_atual = 0;
    *subs = 0;

    for(int i = 0; i < NUMERO_FRAMES; i++)
        limpaFrame(i);

    msg = inicializaMsg();

}
