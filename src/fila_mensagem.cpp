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

void enviaMsg(int id, mensagem msg){
    int tamanho, erro, tentativa = 0;

    if(msg.pagina >= 0)
        msg.mtype = msg.pagina + 1;
    else
        msg.mtype = 1;

    tamanho = sizeof(mensagem) - sizeof(long);

    do{
        erro = msgsnd(id, &msg, tamanho, 0);

        if(erro == -1){
            nanosleep((const struct timespec[]){0, 500000000L}, NULL); //Dorme por 0,5s
            tentativa++;
        }

        if(tentativa == 10)
            break;

    }while(erro == -1);

    if(erro < 0){
        cout << "Erro ao enviar mensagem" << endl;
        exit(EXIT_FAILURE);
    }

}

mensagem recebeMsg(int id){
    mensagem msg;
    int tamanho, erro, tentativa = 0;

    tamanho = sizeof(mensagem) - sizeof(long);

    do{
        erro = msgrcv(id, &msg, tamanho, 0, 0);

        if(erro == -1){
            nanosleep((const struct timespec[]){0, 500000000L}, NULL); //Dorme por 0,5s
            tentativa++;
        }

        if(tentativa == 10)
            break;
    }while(erro == -1);

    if(erro == -1){
        cout << "Erro ao receber mensagem" << endl;
        exit(EXIT_FAILURE);
    }

    return msg;
}
