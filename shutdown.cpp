#include "shutdown.h"

extern frame *tab;
extern map<int, int> proc;
extern int msg, sem;
extern int *subs, *ocupacao_atual;

void printTabela(){
    cout << "Processo\t Pagina \t Tempo de referencia" << endl;
    for(int i = 0; i < NUMERO_FRAMES; i++){
        if(tab[i].estado == LIVRE)
            cout << "Livre" << endl;
        else
            cout << tab[i].processo << "\t\t\t" << tab[i].pagina << "\t" << tab[i].tempo << endl;
    }
}

void shutdown(){

    if(!proc.empty()){ //Este e o processo alocador
        mensagem snd;
        for(map<int, int>::iterator it = proc.begin(); it != proc.end(); it++){
            snd.processo = it->first;
            snd.page_faults = it->second;

            enviaMsg(msg, snd);
        }
        snd.processo = -1;
        enviaMsg(msg, snd);
    }
    else{
        mensagem rcv;
        int page_faults = 0;

        do{
            rcv = recebeMsg(msg);
            if(rcv.processo != -1){
                cout << "Número de page faults do processo " << rcv.processo << ": " << rcv.page_faults << endl;
                page_faults += rcv.page_faults;
            }
        }while(rcv.processo != -1);

        cout << "Número de page faults total: " << page_faults << endl;
        cout << "Número de execuções do processo de substituição: " << *subs << endl;
        cout << "Configuração final da memória:" << endl;
        printTabela();

        destroiSem(sem);

        destroiMem((void*)ocupacao_atual);
        destroiMem((void*)subs);
        destroiMem((void*)tab);

        destroiMsg(msg);
    }
}
