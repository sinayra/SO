#include "semaforo.h"

struct sembuf operacao[2];

int inicializaSem(){
    int idsem;

    idsem = semget(IPC_PRIVATE, 1, IPC_CREAT | 0x1ff);
    if(idsem < 0){
        cout << "Erro com semget" << endl;
        exit(EXIT_FAILURE);
    }

    operacao[1].sem_num = 0;
    operacao[1].sem_op = 1;
    operacao[1].sem_flg = 0;

    return idsem;
}

void P(int idsem){
    operacao[0].sem_num = 0;
    operacao[0].sem_op = 0;
    operacao[0].sem_flg = 0;

    if(semop(idsem, operacao, 2) < 0){
        cout << "Erro com semop" << endl;
        exit(EXIT_FAILURE);
    }
}

void V(int idsem){
    operacao[0].sem_num = 0;
    operacao[0].sem_op = -1;
    operacao[0].sem_flg = 0;

    if(semop(idsem, operacao, 2) < 0){
        cout << "Erro com semop" << endl;
        exit(EXIT_FAILURE);
    }
}

void destroiSem(int idsem){
    /*union semun sem_union;

    if(semctl(idsem, 0, IPC_RMID, sem_union) < 0){
        cout << "Erro com semctl" << endl;
        exit(EXIT_FAILURE);
    }*/
}
