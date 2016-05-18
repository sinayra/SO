#include "../include/fila_mensagem.h"

int inicializaMsg(){
    int id;

    id = msgget(IPC_PRIVATE, 0666|IPC_CREAT);
    if(id < 0){
        cout << "Erro ao criar fila de mensagem" << endl;
        exit(EXIT_FAILURE);
    }

    return id;
}

void destroiMsg(int id){
    int status;

    status = msgctl(id, IPC_RMID, NULL);
    if(status < 0){
        cout << "Erro ao destruir fila de mensagem" << endl;
        exit(EXIT_FAILURE);
    }
}

mensagem enviaMsg(int id, mensagem msg){
    int tamanho, erro;

    msg.mtype = 1;
    tamanho = sizeof(mensagem) - sizeof(long);
    erro = msgsnd(id, &msg, tamanho, 0);

    if(erro == -1){
        cout << "Erro ao enviar mensagem" << endl;
        msg.alocado = false;
        msg.mtype = -1;
        msg.page_faults = -1;
        msg.pagina = -1;
        msg.processo = -1;
    }

    return msg;
}

mensagem recebeMsg(int id){
    mensagem msg;
    int tamanho, erro;

    tamanho = sizeof(mensagem) - sizeof(long);

    erro = msgrcv(id, &msg, tamanho, 0, 0);

    if(erro == -1){
        cout << "Erro ao receber mensagem" << endl;
        msg.alocado = false;
        msg.mtype = -1;
        msg.page_faults = -1;
        msg.pagina = -1;
        msg.processo = -1;
    }
    return msg;
}
