#include "Biblioteca.hpp"
#include <fstream>

int main() {
    FILE *myfile;
    myfile = fopen ("exemplo.txt","r");
    int N, operacao;
    char dado[WORD_SIZE];
    int **cache, **memoria;
    int hit_count = 0, miss_count = 0;
    float hit_rate = 0, miss_rate = 0;
    int total_reads = 0, total_writes = 0;

    vector<string> entries;

    cache = inicializaCache();
    memoria = inicializaMemoria();

    while (!feof(myfile)) {
        string entry = "";

        fscanf(myfile, "%d %d", &N, &operacao);
        cout << N << " " << operacao << " " << endl;

        entry = to_string(N) + " " + to_string(operacao);
        // Se eh uma operação de escrita
        if (operacao == 1) {
            entry = entry + " ";
            total_writes++;

            fscanf(myfile, "%s", dado);
            
            for (int i = 0; i < 32; i++) { 
                entry = entry + dado[i]; 
            }

            entry = entry + " W";
            escreverDado(N, dado, cache, memoria);
        } else {
            total_reads++;

            if (lerDado(N, cache)) {
                entry = entry + " H";
                hit_count++;
            }
            else {
                entry = entry + " M";
                miss_count++;
            }
        }

        entries.push_back(entry);
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

    // Write to file
    ofstream outfile ("result.txt");

    hit_rate = (float) hit_count / (float) total_reads;
    miss_rate = (float) miss_count / (float) total_reads;

    outfile << "READS: " << to_string(total_reads) << endl;
    outfile << "WRITES: " << to_string(total_writes) << endl;
    outfile << "HITS: " << to_string(hit_count) << endl;
    outfile << "MISSES: " << to_string(miss_count) << endl;
    outfile << "HIT RATE: " << to_string(hit_rate) << endl;
    outfile << "MISS RATE: " << to_string(miss_rate) << endl << endl;

    for (int i = 0; i < entries.size(); i++) {
        outfile << entries[i] << endl;
    }

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