#pragma once
#include <string>
#include <vector>
#include <iostream>

using namespace std;


#define MAX_OCUPACAO 9
#define OCUPACAO_OK 8
#define NUMERO_FRAMES 10

typedef enum{
    LIVRE,
    OCUPADO,
    RESERVADO
}tipoEstado;

///Frame
//linha da tabela de páginas
typedef struct{
    int pagina;
    long int tempo;
    int processo;
    int pid;
    tipoEstado estado;
}frame;

typedef struct{
    long int tempo;
    int index;
}frameAux;

//Divide string s em vetor de string
vector<string> explode(string s, char delimitador);
