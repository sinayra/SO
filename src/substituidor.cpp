#include "../include/substituidor.h"

extern void encerra(int signo);
extern int *subs, *ocupacao_atual;
extern int sem;
extern frame *tab;

void substituidor(){

    signal(SIGUSR2, encerra);
    while(1){
        P(sem);
            if(*ocupacao_atual >= MAX_OCUPACAO){
                (*subs)++;
                substitui();
            }
        V(sem);

        nanosleep((const struct timespec[]){0, 500000000L}, NULL); //Dorme por 1ms
    }
}

void substitui(){
    int j, k = 0;
    vector<frameAux> v(NUMERO_FRAMES, {LONG_MAX, -1});

    for(int i = 0; i < NUMERO_FRAMES; i++){
        if(tab[i].tempo > -1){
            v[k] = {tab[i].tempo, i};
            k++;
        }
    }

    sort(v.begin(), v.end(), ordenaFrame);

    j = 0;
    while(*ocupacao_atual >= OCUPACAO_OK){
        if(tab[v[j].index].estado == OCUPADO){
            tab[v[j].index].estado = RESERVADO;
            limpaFrame(v[j].index);

            (*ocupacao_atual)--;
        }
        j++;
    }
}

