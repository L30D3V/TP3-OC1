#include <iostream>
#include <bitset>
using namespace std;
/* 
######### Começo funções auxiliares
*/
// Converte um string em inteiro
int str_to_int(char string){
    if(string == '1')
        return 1;
    else
        return 0;
}
/* 
######### Fim funções auxiliares
*/

// Memória cache - 64 blocos
// - 16 bytes p/ bloco - 128 bits
// - 4 palavras de 32 bits (4bytes) p/ bloco
//   - ou seja, em cada bloco cabem 4 palavras - precisamos criar um offset então
int** inicializaCache(){
    // Matriz "cache" está na seguinte ordem:
    // Índice seriam 6 bits (0 a 5), 1 bit de validade (6), Offset seriam 2 bits (7 e 8),
    // Tag seria o restante: 32 - 6 - 2 = 24 bits (9 a 32) e
    // 128 bits das palavras (33 a 161)

    int **cache; // cache[64][161]; // 161 = largura = 6 + 1 + 24 + 2 + 128
    cache = (int**) malloc(64 * sizeof(int*));
    for(int i = 0; i < 64; i++)
        cache[i] = (int*) malloc(161 * sizeof(int));

    string temp;
    for(int i = 0; i < 64; i++){
        cache[i][6] = 0; // zera o bit de validade
        temp = bitset<6>(i).to_string(); // converte p/ binário

        for(int bin_index = 0; bin_index < 6; bin_index++){
            cache[i][bin_index] = str_to_int(temp[bin_index]);
        }
    }

    return cache;
}

// Memória de dados - 1024 palavras de 32 bits
// matriz 1024x32
int** inicializaMemoria(){
    //  A VERIFICAR SE IMPLEMENTAÇÃO ESTÁ CORRETA
    int **memoria; // memoria[1024][32];
    memoria = (int**) malloc(1024 * sizeof(int*));
    for(int i = 0; i < 1024; i++)
        memoria[i] = (int*) malloc(32 * sizeof(int));

    return memoria;
}

// escreverDado(){
    // Localização determinada pelo endereço --> (Endereço do bloco) mod (#Blocos na cache)
    // Ex: 5 1 00000000000000000000000000000101
    // 5 % 64 = 5

    // Como colocar na cache:
    // Pegue o valor do endereço (em binário) e faça (dado % 64)
    //  - Assim se obtem (o ÍNDICE) onde colocar o dado na cache
    // Checa se o bloco está vazio
    // Salve a TAG
// }