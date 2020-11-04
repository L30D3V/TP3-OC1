#include "Biblioteca.hpp"
// # Implementar - Hierarquia de memória
// - Ler um arquivo, decodificar a requisição da CPU, realizar a operação e retornar um txt com alguns dados simples

int main() {
    FILE *myfile;
    myfile = fopen ("exemplo.txt","r");
    int N, operacao;
    char dado[32];
    int **cache, **memoria;

    cache = inicializaCache();
    memoria = inicializaMemoria();

    while (!feof(myfile)) {
        fscanf(myfile, "%d %d", &N, &operacao);
        cout << N << " " << operacao << " " << endl;
        // Se eh uma operação de escrita
        if(operacao == 1) {
            fscanf(myfile, "%s", dado);
            escreverDado(N, dado, cache);
        } else {
            // lerDado();
        }
    }
    
    // Visualiza a cache
    for (int i = 0; i < 64; i++) {
        cout << ">> " << i << " ";

        for (int j = 0; j < 220; j++) {
            cout << cache[i][j];
            if (j == 0 || j == 22 || j == 54 || j == 55 || j == 77 || j == 109 || j == 110 || j == 132 || j == 164 || j == 165 || j == 187)
                cout << "|";
        }
        cout << endl;
    }

    // Libera a memória da cache e da memoria
    for (int i = 0; i < 64; i++)
        free(cache[i]);
    free(cache);
    
    for (int i = 0; i < 1024; i++)
        free(memoria[i]);
    free(memoria);
    
    fclose(myfile);
    
    return 0;
}