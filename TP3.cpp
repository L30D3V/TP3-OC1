#include "Biblioteca.hpp"

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
        cout << ">> " << i << " |";

        for (int j = 0; j < 228; j++) {
            cout << cache[i][j];
            if (j == 0 || j == 24 || j == 56 || j == 57 || j == 81 || j == 113 || j == 114 || j == 138 || j == 170 || j == 171 || j == 195)
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