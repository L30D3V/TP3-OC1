#include <iostream>
#include <bitset>
using namespace std;

/************** 
###### Começo funções auxiliares ######
*************/

// Converte uma string em inteiro
int str_to_int(char string){
    if(string == '1')
        return 1;
    else
        return 0;
}

// Converte uma string (binário) em inteiro
char bin_to_int(char *dado){
    int offset;
    // Pega as posições 24 e 25 e calcula o offset da palavra
    if(dado[24] == '0'){
        dado[25] == '0' ? offset = 0 : offset = 1;
    }else{
        dado[25] == '0' ? offset = 2 : offset = 3;
    }
    return offset;
}

/*************
###### Fim funções auxiliares ######
*************/

// Memória cache - 64 blocos
// - 16 bytes p/ bloco - 128 bits
// - 4 palavras de 32 bits (4 bytes) por bloco
// - Cache = [ bit_v + tag + palavra |  bit_v + tag + palavra | bit_v + tag + palavra | bit_v + tag + palavra ]
// - 55 bits por palavra = 220 bits no total
int** inicializaCache(){
    // Matriz "cache" está na seguinte ordem:
    // 1 bit de validade, 22 bits de Tag, 32 bits para palavra
    // [0]   -> bit_v1; [1-22]    -> tag_1; [23-54]   -> palavra_1
    // [55]  -> bit_v2; [56-77]   -> tag_2; [78-109]  -> palavra_2
    // [110] -> bit_v3; [111-132] -> tag_3; [133-164] -> palavra_3
    // [165] -> bit_v4; [166-187] -> tag_4; [188-219] -> palavra_3

    int **cache; // cache[64][220]; // 220 = largura = 4 * (1 + 22 + 32)
    cache = (int**) malloc(64 * sizeof(int*));
    for(int i = 0; i < 64; i++)
        cache[i] = (int*) malloc(220 * sizeof(int));

    // Limpar memória cache
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 220; j++) {
            cache[i][j] = 0;
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

void escreverDado(int endereco, char *dado, int **cache){
    // Localização determinada pelo endereço --> (Endereço do bloco) mod (#Blocos na cache)
    // Ex: 5 1 00000000000000000000000000000101
    // 5 % 64 = 5
    int localizacao, offset;
    localizacao = endereco % 64;
    offset = bin_to_int(dado);
    // ##### FALTA:
    // Checar se o bloco está vazio -  e se o bloco estiver cheio mas os outros estiverem vazios? armazeno nos outros ou mando pra memória?
    // Salvar a TAG
    // Alterar o bit de validade
    
    // armazena o dado de acordo com o offset
    // se offset = 0, armazena na cache de 33 a 65, se offset = 1, armazena na cache de 66 a 98, etc
    for(int j = 33*(offset+1), i = 0; j < (33+32*(offset+1)); i++, j++)
        cache[localizacao][j] = str_to_int(dado[i]);
    
    // #### Testando
    // for(int j = 33*(offset+1); j < (33+32*(offset+1)); j++)
    //     cout << cache[localizacao][j];
    // cout << endl;
}