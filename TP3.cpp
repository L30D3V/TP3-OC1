#include "Biblioteca.hpp"
// # Implementar - Hierarquia de memória
// - Ler um arquivo, decodificar a requisição da CPU, realizar a operação e retornar um txt com alguns dados simples

int main(){
    FILE *myfile;
    myfile = fopen ("exemplo.txt","r");
    int N, operacao;
    char dado[32] = "";
    int **cache, **memoria;

    cache = inicializaCache();
    memoria = inicializaMemoria();

    // testando a cache - imprimindo os 7 primeiros bits
    // for(int i = 0; i < 64; i++){
    //     for(int j = 0; j < 7; j++)
    //         cout<< cache[i][j];
    //     cout << endl;
    // }
    // while( !feof(myfile) ){
    //     fscanf(myfile, "%d %d", &N, &operacao);
    //     cout << N << " " << operacao << " ";
    //     // Se eh uma operação de escrita
    //     if(operacao == 1){
    //         fscanf(myfile, "%s", dado); // deixar armazenado como string msm ?

    //         // escreverDado();
    //         // cout << dado << endl;
    //     }else{
    //         // cout << endl;
    //         // lerDado();
    // }

    // libera a memória da cache e da memoria
    for (int i = 0; i < 64; i++)
        free(cache[i]);
    free(cache);
    for (int i = 0; i < 1024; i++)
        free(memoria[i]);
    free(memoria);
    fclose(myfile);
    return 0;
}