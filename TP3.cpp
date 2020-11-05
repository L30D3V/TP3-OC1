#include "Biblioteca.hpp"
#include <fstream>

/*
    A main executa as seguintes etapas:
    - Inicializa a cache
    - Inicializa a memória
    - Lê cada linha do arquivo exemplo.txt
    - Executa a operação de leitura ou escrita e calcula o número de hits e miss
    - Por fim, exibe o relatório no arquivo results.txt
*/
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

        fscanf(myfile, "%d %d\n", &N, &operacao);

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

    // Write to file
    ofstream outfile ("result.txt");
    // dígito de precisão igual a especificação
    outfile.precision(1);

    hit_rate = (float) hit_count / (float) total_reads;
    miss_rate = (float) miss_count / (float) total_reads;

    outfile << "READS: " << to_string(total_reads) << endl;
    outfile << "WRITES: " << to_string(total_writes) << endl;
    outfile << "HITS: " << to_string(hit_count) << endl;
    outfile << "MISSES: " << to_string(miss_count) << endl;
    outfile << "HIT RATE: " << fixed << hit_rate << endl;
    outfile << "MISS RATE: " << fixed << miss_rate << endl << endl;

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