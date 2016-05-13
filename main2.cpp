#include "pagina.h"
#include "fila_mensagem.h"
#include "utils.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>

using namespace std;
int fila_alocar, fila_alocado;

//Vetor com pids de todos os processos que vão referenciar páginas
vector<pid_t> pid_procs;

void substituidor(){
    /*while(1)
        substitui();
        */
    cout << "SOu o substituidor" << endl;
}

void processo(int proc){
    mensagem msg_s;
    cout << "SOu o processo " << proc << endl;

    msg_s.alocado = false;
    msg_s.processo = proc;
    cout << "vai enviar a mensagem para gerenciador: " << "\tproc: " << msg_s.processo << endl;
    cout << "FILA ALOCAR3: " << fila_alocar << endl;
    cout.flush();
    enviaMsg(fila_alocar, msg_s);
    sleep(1);


    cout << "Proc" << proc << " conseguiu enviar mensagem para gerenciador" << endl;
    cout.flush();
/*
    cout << "Proc" << proc << " vai esperar resposta do gerenciador" << endl;
    msg_r = recebeMsg(fila_alocado);
    cout << "Proc" << proc << " recebeu mensagem!\t Processo: " << msg_r.processo << "\tAlocado: " << msg_r.alocado << endl;
*/
    /*ifstream processo;
    string linha;
    stringstream nome;
    vector<string> paginas;

    nome << "pag_processo_" << proc;
    processo.open(nome.str().c_str());
    defineProcesso(proc);

    cout << "Processo " << proc << " vai comecar execucao" << endl;

    if(!processo.is_open()){
        cout << "Erro ao abrir o arquivo " << nome.str() << "Encerrando..." << endl;
        exit(EXIT_FAILURE);
    }

    getline(processo, linha);
    paginas = explode(linha, ',');
    processo.close();

    cout << "Processo " << proc << " leu do arquivo as paginas" << endl;

    for(vector<string>::iterator it = paginas.begin(); it != paginas.end(); it++){
        string::size_type sz;
        int pag = stoi(*it, &sz);
        cout << "Processo " << proc << ": referencia_pagina(" << pag << ")" << endl;

        referencia_pagina(pag);
        //sleep(1);
    }

    cout << "Processo " << proc << " terminou execucao" << endl;
    */
}

void alocador(){
    mensagem msg_r, msg_s;
    cout << "SOu o alocador" << endl;

    while(pid_procs.size() > 0){
        cout.flush();
        cout << "vai receber mensagem para alocar" << endl;
        cout << "FILA ALOCAR2: " << fila_alocar << endl;
        msg_r = recebeMsg(fila_alocar);
        cout << "Recebeu mensagem de proc = " << msg_r.processo << endl;
/*
        msg_s = msg_r;
        msg_s.alocado = true;

        cout << "Gerenciador vai mandar mensagem para processo " << msg_r.processo << endl;
        enviaMsg(fila_alocado, msg_s);
        */
    }
}

//
// id[0]: id_subs
// id[1]: id_page_faults
// id[2]: id_ocupacao_atual
// id[3]: id_tab
//
int main2(int argc, char *argv[]){
    int qtd, proc, ids[4];
    pid_t pid = -1, pid_sub = -1, pid_aloca = -1;
    bool flag_aloca;

    cout << "Informe quantos processos irao executar concorrentemente" << endl;
    cin >> qtd;

    inicializaTab(argv[0], ids);
    fila_alocado = inicializaMsg();
    //fila_alocar = inicializaMsg();
    cout << "FILA ALOCAR1: " << fila_alocar << endl;
    //cout << "FILA ALOCADADO1: " << fila_alocado << endl;


    for(proc = 0; proc < 2; proc++){

        pid = fork(); //substituidor
        if(pid == 0)
            break;

    }
    if(pid == 0){
        switch(proc){
            case 0:
                pid_sub = pid;
                flag_aloca = false;
            break;
            case 1:
                pid_aloca = pid;
                flag_aloca = true;
            break;
            default:
                cout << "Erro ao criar processos" << endl;
                return -1;
            break;
        }
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
                cout << "PID DO FILHO: " << getpid() << endl;
                break;
            }
            else{
                pid_procs.push_back(pid);
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
        int status, erro;

        do{
            for(vector<pid_t>::iterator it = pid_procs.begin(); it != pid_procs.end(); it++){
                erro = waitpid(*it, &status, WNOHANG);
                if(erro == -1){
                    cout << "Erro ao esperar morte do filho " << *it << endl;
                    exit(EXIT_FAILURE);
                }
                if(status != 0)
                    sleep(1);
                else
                    pid_procs.erase(it, it);

            }
        }while(!pid_procs.empty());
        /*for(vector<pid_t>::iterator it = pid_procs.begin(); it != pid_procs.end(); it++){
            //espera processos alocadores morrerem
            do{
                erro = waitpid(*it, &status, WNOHANG);
                if(erro == -1){
                    cout << "Erro ao esperar morte do filho " << *it << endl;
                    exit(EXIT_FAILURE);
                }
                //cout << "Filho " << *it << " nao morreu" << endl;
            }while(status != 0);

            /*cout << endl << "***********************" << endl;
            cout << "Filho " << *it << " morreu" << endl;
            cout << endl << "***********************" << endl;*/

        //}
        destroiMsg(fila_alocar);
        //destroiMsg(fila_alocado);
        //kill(pid_sub, SIGTERM); //pai mata processo substituidor
    }
//cout << "proc " << proc << " vai dar shutdown" << endl;
    //shutdown();
    //destroiMsg(fila_alocado);

    return 0;
}
