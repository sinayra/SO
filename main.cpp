#include "pagina.h"
#include "utils.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

using namespace std;

int main(){
    int qtd, proc;
    ifstream processo;

    inicializaTab();

    cout << "Informe quantos processos irao executar concorrentemente" << endl;
    cin >> qtd;
    qtd--;
    proc = qtd;

    while(qtd >= 0){
        string linha;
        stringstream nome;
        vector<string> paginas;
        nome << "pag_processo_" << qtd;
        processo.open(nome.str().c_str());

        if(!processo.is_open()){
            cout << "Erro ao abrir o arquivo " << nome.str() << "Encerrando..." << endl;
            exit(EXIT_FAILURE);
        }

        getline(processo, linha);
        paginas = explode(linha, ',');

        for(vector<string>::iterator it = paginas.begin(); it != paginas.end(); it++){
            string::size_type sz;
            int pag = stoi(*it, &sz);

            referencia_pagina(pag);
        }

        processo.close();
        qtd--;
    }

    shutdown(proc);

    return 0;
}
