#include "semaforo.h"

struct sembuf operacao;

int inicializaSem(){
    int idsem;
    union semun sem_union;

    idsem = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if(idsem < 0){
        cout << "Erro com semget" << endl;
        exit(EXIT_FAILURE);
    }

    sem_union.val = 1;
    if(semctl(idsem, 0, SETVAL, sem_union) < 0){
        cout << "Erro com semclt" << endl;
        exit(EXIT_FAILURE);
    }
    return idsem;
}

void P(int idsem){
    operacao.sem_num = 0;
    operacao.sem_op = -1; //P()
    operacao.sem_flg = SEM_UNDO;

    if(semop(idsem, &operacao, 1) < 0){
        cout << "Erro com semop" << endl;
        exit(EXIT_FAILURE);
    }
}

void V(int idsem){
    operacao.sem_num = 0;
    operacao.sem_op = 1; //V()
    operacao.sem_flg = SEM_UNDO;

    if(semop(idsem, &operacao, 1) < 0){
        cout << "Erro com semop" << endl;
        exit(EXIT_FAILURE);
    }
}

void finalizaSem(int idsem){
    union semun sem_union;

    sem_union.val = 1;
    if(semctl(idsem, 0, IPC_RMID, sem_union) < 0){
        cout << "Erro com semctl" << endl;
        exit(EXIT_FAILURE);
    }
}
