#include "utils.h"

vector<string> explode(string s, char delimitador){
    vector<string> v;
    string buffer;
    int tam = s.size();

    for(int i = 0; i < tam; i++){
        buffer.clear();

        while(i < tam && s[i] != delimitador){
            buffer += s[i];
            i++;
        }
        v.push_back(buffer);
    }

    return v;
}
