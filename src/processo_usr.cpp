#include "../include/processo_usr.h"

extern int req, resp;

bool referencia_pagina(int i, int processo){
    mensagem msg_r, msg_s;
    msg_s.alocado = false;
    msg_s.processo = processo;
    msg_s.pagina = i;

    enviaMsg(req, msg_s);
    //cout << "Processo" << msg_s.processo << "enviou mensagem" << endl;

    msg_r = recebeMsg(resp, (processo + 1) * 2); //numero magico para recuperar mensagem

    if(!msg_r.alocado || msg_r.processo != processo)
        return false;

    return true;
}

void processo(int proc){
    ifstream fprocesso;
    string linha;
    stringstream nome;
    vector<string> paginas;

    nome << "pag_processo_" << proc;
    fprocesso.open(nome.str().c_str());

    if(!fprocesso.is_open()){
        cout << "Erro ao abrir o arquivo " << nome.str() << endl << "Encerrando processo " << getpid() << "..." << endl;
        exit(EXIT_FAILURE);
    }

    getline(fprocesso, linha);
    paginas = explode(linha, ',');
    fprocesso.close();

    for(vector<string>::iterator it = paginas.begin(); it != paginas.end(); it++){
        string::size_type sz;
        int pag = stoi(*it, &sz);

        while(!referencia_pagina(pag, proc));

    }
    //cout << endl << "Processo " << proc << " terminou execucao" << endl << endl;
}
