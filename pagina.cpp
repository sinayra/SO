#include "pagina.h"

//globais dos processos
//int proc_page_fault = 0;
map<int, int> proc;
int proc_atual = -1;
int msg, sem_proc, sem_escreve;
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

    sem_proc = inicializaSem();
    sem_escreve = inicializaSem();

    *ocupacao_atual = 0;
    *subs = 0;

    for(int i = 0; i < NUMERO_FRAMES; i++)
        limpaFrame(i);

    msg = inicializaMsg();

}

void substitui_manual(){
    int j, k = 0;
    vector<frameAux> v(MAX_OCUPACAO);

    P(sem_escreve);

    for(int i = 0; i < NUMERO_FRAMES; i++){
        if(tab[i].tempo > -1){
            //cout << "k: " << k << endl;
            v[k] = {tab[i].tempo, i};
            k++;
        }
    }

    sort(v.begin(), v.end(), ordenaFrame);

    j = 0;
    while(*ocupacao_atual >= OCUPACAO_OK){
        //cout << "v[j].index: " << v[j].index << endl;
        if(tab[v[j].index].estado == OCUPADO){
            tab[v[j].index].estado = RESERVADO;
            limpaFrame(v[j].index);

            (*ocupacao_atual)--;
        }
        j++;

        //cout << "limpando ocupacao atual: " << *ocupacao_atual << endl;
    }

    V(sem_escreve);
}

void substitui(){

    while(1){
        cout.flush();
        cout << "ocupacao atual substitui: " << *ocupacao_atual << endl << endl;
        if(*ocupacao_atual >= MAX_OCUPACAO){
            //cout << endl << "---------------------" << endl << "Substituidor!" << endl;
                //printTabela();
                //cout << endl << "---------------------" << endl;
            (*subs)++;
            substitui_manual();
        }
        //else
            //sleep(1);
    }

}

void aloca(int i){
    int j;


    int index = getPagina(i);

    if(index < 0){
        cout << "ocupacao atual aloca: " << *ocupacao_atual << "\tNUMERO_FRAMES" << NUMERO_FRAMES << endl;
        //cin.get();
        //cout << endl << "********************************" << endl;
        //cout << "Antes de colocar pagina " << i << " do processo " << proc << endl;
        //printTabela();
        if(*ocupacao_atual >= NUMERO_FRAMES){
            cout << endl << "---------------------" << endl << "SUBS!" << endl;
            cout.flush();
            substitui_manual();
            //printTabela();
            cout << endl << "---------------------" << endl;
        }
        cout << "NAO ENTROU NO IF" << endl;
        P(sem_escreve);
        /*j = 0;
        while(tab[j].estado != LIVRE)
            j++;*/
        do{
            j = rand()%NUMERO_FRAMES;
        }while(tab[j].estado != LIVRE);
        preencheFrame(j, i);

        (*ocupacao_atual)++;

        proc[proc_atual]++;

        V(sem_escreve);

        //cout << endl << "depois de tudo, ocupacao atual: " << *ocupacao_atual << endl;

        //cout << endl << "Depois de colocar pagina " << i << " do processo " << proc << endl;
        //printTabela();
        //cout << "********************************" << endl;
        //cin.get();
    }
    else{
        preencheFrame(index, i);
    }
}

void referencia_pagina(int i, int processo){
    cout << "P() para processo " << processo << endl;
    P(sem_proc);
    //cout << "entrou em referencia_pagina" << endl;
        map<int, int>::iterator it = proc.begin();
        proc_atual = processo;
        proc.insert(it, pair<int, int>(processo, 0)); //se á tiver esse processo, não insere

        //cout << "processo: " << (proc.begin())->first << "\t qtd: " << (proc.begin())->second << endl;
        aloca(i);
    //cout << "saiu de aloca" << endl;
    cout << "V() para processo " << processo  << endl;
    V(sem_proc);
}
