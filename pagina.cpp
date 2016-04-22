#include "pagina.h"

//Memória compartilhada
//frame tab[NUMERO_FRAMES];
int ocupacao_atual = 0;
int page_fault = 0;
int subs = 0;


//globais do processo atual
int proc;
int proc_page_fault = 0;
int id_subs, id_page_faults, id_ocupacao_atual, id_tab;

void limpaFrame(int i){
    frame *mc_tab;

    mc_tab = (frame *)shmat(id_tab, (char*)0, 0);
    if(mc_tab == (frame*)-1){
        cout << "Erro em shmat() 1" << endl;
        exit(EXIT_FAILURE);
    }

    mc_tab[i].livre = true;
    mc_tab[i].pagina = -1;
    mc_tab[i].processo = -1;
    mc_tab[i].tempo = -1;
    mc_tab[i].pid = -1;
}

void preencheFrame(int i, int pagina){
    struct timeval t;
    frame *mc_tab;

    mc_tab = (frame *)shmat(id_tab, (char*)0, 0);
    if(mc_tab == (frame*)-1){
        cout << "Erro em shmat() 2" << endl;
        exit(EXIT_FAILURE);
    }

    mc_tab[i].livre = false;
    mc_tab[i].pagina = pagina;
    mc_tab[i].processo = proc;
    mc_tab[i].pid = getpid();

    gettimeofday(&t, NULL);
    mc_tab[i].tempo = t.tv_sec * 1000000L + t.tv_usec;

}

bool ordenaFrame(frameAux v1, frameAux v2){
    return v1.tempo < v2.tempo;
}

int getPagina(int i){
    frame *mc_tab;

    cout << "idtab: " << id_tab << endl;
    mc_tab = (frame *)shmat(id_tab, 0, 0);
    if(mc_tab == (frame*)-1){
        cout << "Erro em shmat() 3" << endl;
        exit(EXIT_FAILURE);
    }

    for(int j = 0; j < NUMERO_FRAMES; j++){
        if(mc_tab[j].processo == proc && mc_tab[j].pagina == i)
            return j;
    }
    return -1;
}

void defineProcesso(int processo, int ids[4]){
    proc = processo;
    id_subs = ids[0];
    id_page_faults = ids[1];
    id_ocupacao_atual = ids[2];
    id_tab = ids[3];
}

void inicializaTab(char *path, int ids[4]){
    key_t k;

    k = ftok(path, 'S');
    if(k < 0){
        cout << "Erro com ftok()" << endl;
        exit(EXIT_FAILURE);
    }

    //ids[0] = shmget(k, sizeof(int), IPC_CREAT);
    //ids[1] = shmget(k, sizeof(int), IPC_CREAT);
    //ids[2] = shmget(k, sizeof(int), IPC_CREAT);
    ids[0] = 1;
    ids[1] = 1;
    ids[2] = 1;
    ids[3] = shmget(k, sizeof(frame) * NUMERO_FRAMES, IPC_CREAT|0x1ff);

    if(ids[0] == -1 || ids[1] == -1 || ids[2] == -1 || ids[3] == -1){
        cout << "Erro com shmget()" << endl;
        exit(EXIT_FAILURE);
    }


    for(int i = 0; i < NUMERO_FRAMES; i++)
        limpaFrame(i);
}

void aloca(int i){
    int j;
    int pag = getPagina(i);
    frame *mc_tab;

    mc_tab = (frame *)shmat(id_tab, (char*)0, 0);
    if(mc_tab == (frame*)-1){
        cout << "Erro em shmat() 4" << endl;
        exit(EXIT_FAILURE);
    }

    if(pag < 0){
        if(ocupacao_atual > MAX_OCUPACAO){
            substitui(i);
        }
        else{
            do{
                j = rand()%NUMERO_FRAMES;
            }while(!mc_tab[j].livre);

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
    frame *mc_tab;

    mc_tab = (frame *)shmat(id_tab, (char*)0, 0);
    if(mc_tab == (frame*)-1){
        cout << "Erro em shmat() 5" << endl;
        exit(EXIT_FAILURE);
    }

    subs++;

    for(int i = 0; i < NUMERO_FRAMES; i++)
        v[i] = {mc_tab[i].tempo, i};

    sort(v.begin(), v.end(), ordenaFrame);

    j = 0;
    while(ocupacao_atual >= OCUPACAO_OK){
        limpaFrame(v[j].index);
        j++;
        ocupacao_atual--;
    }

    do{
        j = rand()%NUMERO_FRAMES;
    }while(!mc_tab[j].livre);

    preencheFrame(j, i);
    proc_page_fault++;
    page_fault++;
    ocupacao_atual++;
}

void shutdownFinal(){
    frame *mc_tab;

    mc_tab = (frame *)shmat(id_tab, (char*)0, 0);
    if(mc_tab == (frame*)-1){
        cout << "Erro em shmat() 6" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Número de page faults total: " << page_fault << endl;
    cout << "Número de execuções do processo de substituição: " << subs << endl;
    cout << "Configuração final da memória:" << endl;

    cout << "Processo\t Pagina \t Tempo de referencia" << endl;
    for(int i = 0; i < NUMERO_FRAMES; i++){
        if(mc_tab[i].livre)
            cout << "Livre" << endl;
        else
            cout << mc_tab[i].processo << "\t\t\t" << mc_tab[i].pagina << "\t" << mc_tab[i].tempo << endl;
    }
}

void shutdown(int pid){

    if(pid == 0)
        cout << "Número de page faults do processo " << proc << ": " << proc_page_fault << endl;
    else{
        frame *mc_tab;

        mc_tab = (frame *)shmat(id_tab, (char*)0, 0);
        if(mc_tab == (frame*)-1){
            cout << "Erro em shmat() 7" << endl;
            exit(EXIT_FAILURE);
        }

        shutdownFinal();
        //shmdt(id_ocupacao_atual);
        //shmdt(id_page_faults);
        //shmdt(id_subs);
        shmdt(mc_tab);
    }
}

void referencia_pagina(int i){
    aloca(i);
}
