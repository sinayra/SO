#include "pagina.h"
#include "utils.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include <unistd.h>
#include <sys/wait.h>

using namespace std;

//
// id[0]: id_subs
// id[1]: id_page_faults
// id[2]: id_ocupacao_atual
// id[3]: id_tab
//
int main(int argc, char *argv[]){
    int qtd, proc, ids[4];
    ifstream processo;
    pid_t pid;

    cout << "Informe quantos processos irao executar concorrentemente" << endl;
    cin >> qtd;

    inicializaTab(argv[0], ids);
    for(proc = 0; proc < qtd; proc++){
        pid = fork();

        if(pid == -1){
            cout << "Erro ao criar processo" << endl;
            exit(EXIT_FAILURE);
        }

        else if(pid == 0){
            cout << "Novo filho. Proc: " << proc << endl;
            break;
        }
    }

    if(pid == 0){
        string linha;
        stringstream nome;
        vector<string> paginas;

        nome << "pag_processo_" << proc;
        processo.open(nome.str().c_str());
        defineProcesso(proc, ids);

        cout << "Processo " << proc << " vai comecar execucao" << endl;

        if(!processo.is_open()){
            cout << "Erro ao abrir o arquivo " << nome.str() << "Encerrando..." << endl;
            exit(EXIT_FAILURE);
        }

        getline(processo, linha);
        paginas = explode(linha, ',');

        cout << "Processo " << proc << " leu do arquivo as paginas" << endl;

        for(vector<string>::iterator it = paginas.begin(); it != paginas.end(); it++){
            string::size_type sz;
            int pag = stoi(*it, &sz);
            cout << "Processo " << proc << ": referencia_pagina(" << pag << ")" << endl;

            referencia_pagina(pag);
        }

        processo.close();
        shutdown(pid);

        cout << "Processo " << proc << " terminou execucao" << endl;
        exit(EXIT_SUCCESS);
    }
    else{
        int status, wpid;

        do{
            wpid = wait(&status);
        }while(wpid > 0);
    }

    shutdown(pid);
    return 0;
}
