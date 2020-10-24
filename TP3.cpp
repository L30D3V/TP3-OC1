#include <iostream>
#include "Biblioteca.hpp"
using namespace std;

// # Implementar - Hierarquia de memória
// - Ler um arquivo, decodificar a requisição da CPU, realizar a operação e retornar um txt com alguns dados simples

int main(){
    FILE *myfile;
    myfile = fopen ("exemplo.txt","r");
    int N, operacao;
    char dado[32] = "";

    // inicializaMemoria();
    // inicializaCache();
    
    while( !feof(myfile) ){
        fscanf(myfile, "%d %d", &N, &operacao);
        cout << N << " " << operacao << " ";
        // Se eh uma operação de escrita
        if(operacao == 1){
            fscanf(myfile, "%s", dado); // deixar armazenado como string msm ?

            // escreverDado();
            cout << dado << endl;
        }else{
            cout << endl;
            // lerDado();
    }
    fclose(myfile);
    return 0;
}