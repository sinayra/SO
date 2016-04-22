/*
#include "pagina.h"
#include "utils.h"
#include <sstream>
#include <string>
*/
#include <fstream>
#include <iostream>
//Chamadas de sistemas
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/sem.h>

using namespace std;

int main(int argc, char* argv[]){
    int qtd, i, idshm, *pshm;
    ifstream processo;
    pid_t pid;
    key_t k;

    //cout << "Informe quantos processos irao executar concorrentemente" << endl;
    //cin >> qtd;
    //qtd--;

    k = ftok(argv[0], 'S');
    if(k < 0){
        cout << "Erro com ftok()" << endl;
        exit(EXIT_FAILURE);
    }
    idshm = shmget(k, sizeof(int), IPC_CREAT);
    if(idshm == -1){
        cout << "Erro com shmget()" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "argv[0]: "  << argv[0] << endl;
    cout << endl << "Vou comecar execucao agora" << endl;
    for(i = 0; i < 3; i++){
        pid = fork();

        if(pid == -1){
            cout << "Erro" << endl;
            exit(EXIT_FAILURE);
        }
        else if(pid == 0){
            break;
        }
        cout << "Meu filho " << i << " saiu do loop. Vou dormir por 5 segundos" << endl << endl;
        sleep(5);
    }

    if(pid == 0){
        cout << "Sou o filho " << i << " e meu pid e " << getpid() <<  endl << endl;
        pshm = (int *)shmat(idshm, (char*)0, 0); //0 para procesos escolher onde salvar e 0 é para read/write
        if(pshm == (int*)-1){
            cout << "Erro em shmat()" << endl;
            exit(EXIT_FAILURE);
        }
        sleep(10);
        (*pshm) = i;
        cout << "Escrevi " << i << " na memoria compartilhada depois de 10 segundos" << endl;
        exit(EXIT_SUCCESS);
    }

    cout << "sou o pai e vou dormir por 5 segundos" << endl << endl;
    sleep(5);
    pshm = (int *)shmat(idshm, (char*)0, 0);
    cout << "Sou o pai e terminei execucao lendo " << *pshm << " da memoria compartilhada" << endl;
    shmdt(pshm);

    return 0;
}
