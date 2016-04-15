#include "pagina.h"
#include <fstream>
#include <sstream>
using namespace std;

int main(){
    int qtd;
    ifstream processo;

    cout << "Informe quantos processos irao executar concorrentemente" << endl;
    cin >> qtd;

    qtd--;

    while(qtd >= 0){
        string linha;
        stringstream nome;
        nome << "pag_processo_" << qtd;
        processo.open(nome.str().c_str());

        if(!processo.is_open()){
            cout << "Erro ao abrir o arquivo " << nome << endl << "Encerrando..." << endl;
            exit(EXIT_FAILURE);
        }

        getline(processo, linha);
        cout << linha;

        processo.close();
        qtd--;
    }

    return 0;
}
