#include "../include/alocador.h"

extern int req, resp;
map<int, int> proc;

extern void encerra(int signo);
extern int sem, proc_atual;
extern int *ocupacao_atual;
extern frame *tab;

void alocador(){
    mensagem msg_r, msg_s;
    map<int, int>::iterator it;

    signal(SIGUSR1, encerra);

    while(1){
        msg_r = recebeMsg(req);

        it = proc.begin();
        proc.insert(it, pair<int, int>(msg_r.processo, 0)); //se ja tiver esse processo, nao insere

        proc_atual = msg_r.processo;  //determina que o processo atual e o da mensagem recebida
        aloca(msg_r.pagina); //realiza operacao de alocar

        msg_s = msg_r;
        msg_s.alocado = true;
        enviaMsg(resp, msg_s, (msg_s.processo + 1)*2);
        //cout << "Alocador enviou mensagem" << endl;
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
