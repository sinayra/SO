#include "include/fila_mensagem.h"
#include "include/utils.h"
#include "include/pagina.h"
#include "include/alocador.h"
#include "include/shutdown.h"
#include "include/processo_usr.h"

#include <string>
#include <iostream>
#include <vector>

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

///Cria/Destrói processos e mecanismos de sincronização
int main(){
    int qtd, proc;
    pid_t pid = -1, pid_sub = -1, pid_aloca = -1;
    bool flag_aloca;

    cout << "Informe quantos processos irao executar concorrentemente" << endl;
    do{
        cin >> qtd;
        if(qtd < 1)
            cout << "Erro! Informe numero valido de processos que executaram concorrentemente" << endl;
        cout.flush();
    }while(qtd < 1);

    inicializaMemVirtual();
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
            p = waitpid(-1, &status, WNOHANG);

            if(p == -1){
                cout << "Erro ao esperar morte do filho " << p << endl;
                exit(EXIT_FAILURE);
            }
            if(!WIFEXITED(status))
                sleep(1);
            else
                qtd--;

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
