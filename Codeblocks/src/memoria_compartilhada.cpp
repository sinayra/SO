#include "../include/memoria_compartilhada.h"

int inicializaMemInt(int num){
    int id;

    id = shmget(IPC_PRIVATE, sizeof(int) * num, 0666 | IPC_CREAT);

    if(id == -1){
        cout << "Erro com shmget" << endl;
        exit(EXIT_FAILURE);
    }

    return id;
}

int inicializaMemFrame(int num){
    int id;

    id = shmget(IPC_PRIVATE, sizeof(frame) * num, 0666 | IPC_CREAT);

    if(id == -1){
        cout << "Erro com shmget" << endl;
        exit(EXIT_FAILURE);
    }

    return id;
}

void associaMemFrame(frame **mem, int id){
    (*mem) = (frame *)shmat(id, 0, 0);
    if((*mem) == (frame*)-1){
        cout << "Erro em shmat()" << endl;
        exit(EXIT_FAILURE);
    }
}

void associaMemInt(int **mem, int id){
    (*mem) = (int *)shmat(id, 0, 0);
    if((*mem) == (int*)-1){
        cout << "Erro em shmat()" << endl;
        exit(EXIT_FAILURE);
    }
}

void destroiMem(void *mem){
    shmdt(mem);
}
