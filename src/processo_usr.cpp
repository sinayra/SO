#include "../include/processo_usr.h"

extern int req, resp;

void processo(int proc){
    mensagem msg_r, msg_s;
    ifstream processo;
    string linha;
    stringstream nome;
    vector<string> paginas;

    nome << "pag_processo_" << proc;
    processo.open(nome.str().c_str());

    if(!processo.is_open()){
        cout << "Erro ao abrir o arquivo " << nome.str() << endl << "Encerrando processo " << getpid() << "..." << endl;
        exit(EXIT_FAILURE);
    }

    getline(processo, linha);
    paginas = explode(linha, ',');
    processo.close();

    for(vector<string>::iterator it = paginas.begin(); it != paginas.end(); it++){
        string::size_type sz;
        int pag = stoi(*it, &sz);

        msg_s.alocado = false;
        msg_s.processo = proc;
        msg_s.pagina = pag;

        enviaMsg(req, msg_s);
        //cout << "Processo " << proc << ": referencia_pagina(" << pag << ")" << endl;

        msg_r = recebeMsg(resp, (pag + 1) * 2); //numero magico para recuperar mensagem
        //cout <<"Alocado: Processo " << msg_r.processo << "\t pagina " << msg_r.pagina << endl;

    }
    //cout << endl << "Processo " << proc << " terminou execucao" << endl << endl;
}
