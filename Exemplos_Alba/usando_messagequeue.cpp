#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/errno.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>

using namespace std;

/*typedef struct{
    long mtype;
    int processo;
    bool alocado;
}mensagem;

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
        //perror("msgclt");
        exit(EXIT_FAILURE);
    }
}

void enviaMsg(int id, mensagem msg){
    int tamanho, erro;

    msg.mtype = 1;
    tamanho = sizeof(mensagem) - sizeof(long);
    erro = msgsnd(id, &msg, tamanho, 0);
   // cout << endl << "enviou mensagem do processo " << msg.processo << endl;
   // cout << "FILA ALOCAR4: " << id << endl;
   // cout.flush();

    if(erro == -1){
        cout << "Erro ao enviar mensagem" << endl;
        //perror("msgsnd");
        exit(EXIT_FAILURE);
    }
}

mensagem recebeMsg(int id){
    mensagem msg;
    int tamanho, erro;

    tamanho = sizeof(mensagem) - sizeof(long);

    erro = msgrcv(id, &msg, tamanho, 0, 0);

    if(erro == -1){
        cout << "Erro ao receber mensagem" << endl;
        //perror("msgrcv");
        exit(EXIT_FAILURE);
    }
    return msg;
}*/

int id1, id2;
int main(){
    pid_t pid;

    id1 = inicializaMsg();
    id2 = inicializaMsg();

    pid = fork();
    if(pid == 0){
        mensagem msg_s, msg_r;
        msg_s.mtype = 1;
        msg_s.processo = 10;
        msg_s.alocado = true;

        //msgsnd(id1, &msg_s, (sizeof(mensagem) - sizeof(long)), 0);
        enviaMsg(id1, msg_s);

        msg_r = recebeMsg(id2);
        cout << msg_r.processo << endl;

        destroiMsg(id2);

    }
    else if(pid > 0){
        mensagem msg_r, msg_s;

        //msgrcv(id1, &msg_r, (sizeof(mensagem) - sizeof(long)), 0, 0);
        msg_r = recebeMsg(id1);
        cout << msg_r.processo << endl;

        msg_s = msg_r;
        msg_s.processo++;
        enviaMsg(id2, msg_s);

        destroiMsg(id1);
    }

    cout << "Encerrou" << endl;
    return 0;
}
