#include "Biblioteca.hpp"

int main() {
    FILE *myfile;
    myfile = fopen ("exemplo.txt","r");
    int N, operacao;
    char dado[WORD_SIZE];
    int **cache, **memoria;
    int hit_count = 0, miss_count = 0;

    cache = inicializaCache();
    memoria = inicializaMemoria();

    while (!feof(myfile)) {
        fscanf(myfile, "%d %d", &N, &operacao);
        cout << N << " " << operacao << " " << endl;
        // Se eh uma operação de escrita
        if(operacao == 1) {
            fscanf(myfile, "%s", dado);
            escreverDado(N, dado, cache, memoria);
        } else {
            if (lerDado(N, cache))
                hit_count++;
            else
                miss_count++;
        }
    }

    // Visualiza a cache
    // for (int i = 0; i < 64; i++) {
    //     cout << ">> " << i << " |";

    //     for (int j = 0; j < 228; j++) {
    //         cout << cache[i][j];
    //         if (j == 0 || j == 24 || j == 56 || j == 57 || j == 81 || j == 113 || j == 114 || j == 138 || j == 170 || j == 171 || j == 195)
    //             cout << "|";
    //     }
    //     cout << endl;
    // }

    // Visualiza hit e miss
    cout << ">> Hits: " << hit_count << " | Miss: " << miss_count << endl;

    // Libera a memória da cache e da memoria
    for (int i = 0; i < CACHE_NUM_BLOCKS; i++)
        free(cache[i]);
    free(cache);

    for (int i = 0; i < MEMORY_NUM_BLOCKS; i++)
        free(memoria[i]);
    free(memoria);

    fclose(myfile);

    return 0;
}