#include "substituidor.h"

extern void encerra(int signo);
extern int *subs, *ocupacao_atual;
extern int sem_escreve;
extern frame *tab;

void substituidor(){

    signal(SIGUSR2, encerra);

    while(1){
    cout << "P SUBSTITUIDOR" << endl;
        P(sem_escreve, 1);
            if(*ocupacao_atual >= MAX_OCUPACAO){
                (*subs)++;
                substitui();
            }
        V(sem_escreve, 1);
        cout << "V SUBSTITUIDOR" << endl;

        nanosleep((const struct timespec[]){0, 1000000}, NULL); //Dorme por 1ms
    }
}

void substitui(){
    int j, k = 0;
    vector<frameAux> v(NUMERO_FRAMES, {LONG_MAX, -1});
    cout << "criou frameaux" << endl;

    for(int i = 0; i < NUMERO_FRAMES; i++){
        if(tab[i].tempo > -1){
            v[k] = {tab[i].tempo, i};
            k++;
        }
    }

    cout << "populou" << endl;

    sort(v.begin(), v.end(), ordenaFrame);

    cout << "sorteou" << endl;

    j = 0;
    while(*ocupacao_atual >= OCUPACAO_OK){
        if(tab[v[j].index].estado == OCUPADO){
            tab[v[j].index].estado = RESERVADO;
            limpaFrame(v[j].index);

            (*ocupacao_atual)--;
        }
        j++;
    }
    cout << "limpou" << endl;
}

