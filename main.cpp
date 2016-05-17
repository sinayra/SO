#include "fila_mensagem.h"
#include "utils.h"
#include "pagina.h"
#include "shutdown.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

//req o processo pede ao alocador para poder alocar
//resp o alocador responde que deu tudo certo ao processo
int req, resp;

void encerra(int signo){
    if(signo == SIGUSR1 || signo == SIGUSR2){
        if(signo == SIGUSR1)
            shutdown();

        exit(EXIT_SUCCESS);
    }

}

void alocador(){
    mensagem msg_r, msg_s;

    signal(SIGUSR1, encerra);
    //cout << "Sou alocador" << endl;
    while(1){
        msg_r = recebeMsg(req);
        //cout << "Recebeu mensagem! Processo " << msg_r.processo << "\tAlocado: " << msg_r.alocado << endl;
        cout << "Alocador recebeu mensagem! Processo " << msg_r.processo << "\tPagina: " << msg_r.pagina << "\tAlocado: " << msg_r.alocado << endl;
        referencia_pagina(msg_r.pagina, msg_r.processo);

        msg_s = msg_r;
        msg_s.alocado = true;
        enviaMsg(resp, msg_s);
    }
}

void substituidor(){
    signal(SIGUSR2, encerra);
    substitui();
}

void processo(int proc){
    mensagem msg_r, msg_s;

    ifstream processo;
    string linha;
    stringstream nome;
    vector<string> paginas;

    nome << "pag_processo_" << proc;
    processo.open(nome.str().c_str());

    cout << "Processo " << proc << " vai comecar execucao" << endl;

    if(!processo.is_open()){
        cout << "Erro ao abrir o arquivo " << nome.str() << "Encerrando..." << endl;
        exit(EXIT_FAILURE);
    }

    getline(processo, linha);
    paginas = explode(linha, ',');
    processo.close();

    //cout << "Processo " << proc << " leu do arquivo as paginas" << endl;

    for(vector<string>::iterator it = paginas.begin(); it != paginas.end(); it++){
        string::size_type sz;
        do{
            int pag = stoi(*it, &sz);

            msg_s.alocado = false;
            msg_s.processo = proc;
            msg_s.pagina = pag;
            enviaMsg(req, msg_s);

            msg_r = recebeMsg(resp);
            cout << "Processo " << proc << ": referencia_pagina(" << pag << ")" << endl;
            cout.flush();
            //cout << "Recebeu mensagem! Processo " << msg_r.processo << "\tAlocado: " << msg_r.alocado << endl;
            //cout << "Processo recebeu mensagem! Processo " << msg_r.processo << "\tPagina: " << msg_r.pagina << "\tAlocado: " << msg_r.alocado << endl;
        }while(!msg_r.alocado);
    }
    //cout << endl << "Processo " << proc << " terminou execucao" << endl << endl;
}

///Cria/Destrói processos e mecanismos de sincronização
int main(){
    int qtd, proc;
    pid_t pid = -1, pid_sub = -1, pid_aloca = -1;
    bool flag_aloca;

    cout << "Informe quantos processos irao executar concorrentemente" << endl;
    cin >> qtd;
    cout.flush();

    inicializaTab();
    req = inicializaMsg();
    resp = inicializaMsg();

    for(proc = 0; proc < 2; proc++){

        pid = fork();
        if(pid == 0)
            break;
        else{
            if(proc == 0)
                pid_sub = pid; //pai guardando pid do substituidor
            else if(proc == 1)
                pid_aloca = pid; //pai guardando pid do alocador
        }

    }
    if(pid == 0){
        pid_aloca = pid_sub = 0;
        if(proc == 0)
            flag_aloca = false; //substituidor
        else if(proc == 1)
            flag_aloca = true; //alocador
    }

    if(pid > 0){ //processo pai vai executar
        for(proc = 0; proc < qtd; proc++){
            pid = fork();

            if(pid == -1){
                cout << "Erro ao criar processo" << endl;
                return -1;
            }

            else if(pid == 0){
                flag_aloca = true;
                break;
            }
        }
    }

    if(pid_sub == 0 && !flag_aloca){
        substituidor();
    }
    else if(flag_aloca && (pid_aloca == 0 || pid == 0)){
        if(pid_aloca == 0){
            alocador();
        }
        else if(pid == 0){
            processo(proc);
        }
    }

    if(pid > 0){ //processo pai vai executar
        int status, p;
        sleep(1);
        do{

            //cout << endl << "Vou esperar status dos filhos" << endl << endl;
            p = waitpid(-1, &status, WNOHANG);
            //cout << "p: " << p << endl;
            //cout << "Filho: " << p << "\tstatus: " << status << endl;
            if(p == -1){
                //cout << "Erro ao esperar morte do filho " << p << endl;
                exit(EXIT_FAILURE);
            }
            if(!WIFEXITED(status)){
                cout << "Filho " << p << " ainda nao morreu" << endl;
                sleep(1);
            }
            else{
                cout << "Filho morto: " << p << "\tstatus: " << status << endl;
                qtd--;
            }

            if(qtd == 0){
                kill(pid_aloca, SIGUSR1);
                kill(pid_sub, SIGUSR2);
                shutdown();
                destroiMsg(req);
                destroiMsg(resp);
            }
        }while(qtd > 0);
    }
    return 0;
}
