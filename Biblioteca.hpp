#include <iostream>
#include <bitset>
#include <vector>
#include <math.h>
using namespace std;

/*************
    Variáveis globais
*************/
const int TAG_SIZE = 24;
const int INDEX_SIZE = 6;
const int OFFSET_SIZE = 2;
const int WORD_SIZE = 32;
const int BIT_V_SIZE = 1;
const int CACHE_BLOCK_PARTITION_SIZE = WORD_SIZE + TAG_SIZE + BIT_V_SIZE; // 57
const int CACHE_BLOCK_SIZE = 4 * CACHE_BLOCK_PARTITION_SIZE; // 228
const int CACHE_NUM_BLOCKS = 64;
const int MEMORY_NUM_BLOCKS = 1024;

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

/*
    Pega a offset à partir do endereço de memória
    2 bits menos significativos
*/
int getOffset(int endereco) {
    bitset<OFFSET_SIZE> bit_offset(endereco);

    return (int)(bit_offset.to_ulong());
}

/*
    Pega a index à partir do endereço de memória
    Do terceiro MSB ao oitavo MSB (6 bits)
*/
int getIndex(int endereco) {
    bitset<INDEX_SIZE> bit_index;
    bitset<8> bit_endereco(endereco);

    bit_index[0] = bit_endereco[2];
    bit_index[1] = bit_endereco[3];
    bit_index[2] = bit_endereco[4];
    bit_index[3] = bit_endereco[5];
    bit_index[4] = bit_endereco[6];
    bit_index[5] = bit_endereco[7];

    return (int)(bit_index.to_ulong());
}

/*
    Pega a tag à partir do endereço de memória
    24 bits mais significativos
*/
string getTag(int endereco) {
    bitset<TAG_SIZE> bit_tag;
    bitset<WORD_SIZE> bit_endereco(endereco);

    for (int i = 0; i < TAG_SIZE; i++) {
        bit_tag[i] = bit_endereco[i+8];
    }

    return bit_tag.to_string();
}

/*
    Pega o dado de um bloco específico da cache e de sua partição (offsets)
*/
int* getDataFromCache(int** cache, int index, int offset) {
    int pos_i = CACHE_BLOCK_PARTITION_SIZE*offset + TAG_SIZE + BIT_V_SIZE;
    int pos_end = pos_i + WORD_SIZE;
    int *data = (int*) malloc(WORD_SIZE * sizeof(int));

    int i = 0;
    while (pos_i <= pos_end) {
        data[WORD_SIZE - 1 - i] = cache[index][pos_i];
        i++;
        pos_i++;
    }

    return data;
}

/*
    Pega a tag de um bloco específico da cache e de sua partição (offsets)
*/
string getTagFromCache(int **cache, int index, int offset) {
    int pos_i = offset*CACHE_BLOCK_PARTITION_SIZE + BIT_V_SIZE;
    int pos_end = pos_i + TAG_SIZE;
    bitset<TAG_SIZE> bit_tag;

    int i = 0;
    while (pos_i <= pos_end) {
        bit_tag[TAG_SIZE - 1 - i] = cache[index][pos_i];
        i++;
        pos_i++;
    }

    return bit_tag.to_string();
}

/*************
###### Fim funções auxiliares ######
*************/

// Memória cache - 64 blocos
// - 16 bytes p/ bloco - 128 bits
// - 4 palavras de 32 bits (4 bytes) por bloco
// - Cache = [ bit_v + tag + palavra |  bit_v + tag + palavra | bit_v + tag + palavra | bit_v + tag + palavra ]
// - 57 bits por palavra = 228 bits no total
int** inicializaCache(){
    // Matriz "cache" está na seguinte ordem:
    // 1 bit de validade, 24 bits de Tag, 32 bits para palavra
    // [0]   -> bit_v1; [1-24]    -> tag_1; [25-56]   -> palavra_1
    // [57]  -> bit_v2; [58-]   -> tag_2; [80-111]  -> palavra_2
    // [112] -> bit_v3; [113-134] -> tag_3; [135-166] -> palavra_3
    // [167] -> bit_v4; [168-189] -> tag_4; [190-221] -> palavra_3

    int **cache; // cache[64][228]; // 228 = largura = 4 * (1 + 24 + 32)
    cache = (int**) malloc(CACHE_NUM_BLOCKS * sizeof(int*));
    for(int i = 0; i < CACHE_NUM_BLOCKS; i++)
        cache[i] = (int*) malloc(CACHE_BLOCK_SIZE * sizeof(int));

    // Limpar memória cache
    for (int i = 0; i < CACHE_NUM_BLOCKS; i++) {
        for (int j = 0; j < CACHE_BLOCK_SIZE; j++) {
            cache[i][j] = 0;
        }
    }

    return cache;
}

/*
    Aloca memória
*/
// Memória de dados - 1024 palavras de 32 bits
// matriz 1024x32
int** inicializaMemoria(){
    int **memoria; // memoria[1024][32];
    memoria = (int**) malloc(MEMORY_NUM_BLOCKS * sizeof(int*));
    for(int i = 0; i < MEMORY_NUM_BLOCKS; i++)
        memoria[i] = (int*) malloc(WORD_SIZE * sizeof(int));

    return memoria;
}
/*
    Recebe um endereço e o dado e faz a escrita na memória
*/
void escreverDadoMemoria(int endereco, int* dado, int** memoria) {
    for(int i = 0; i < WORD_SIZE; i++) {
        memoria[endereco][i] = dado[i];
    }
}

/*
    Recebe um index, o offset e a tag (calculado a partir dos bits de endereço)
    e o dado aí escreve no dado e na tag.
*/
void escreverDadoCache(int** cache, int index, string tag, int offset, char* dado) {
    int block_offset = offset*CACHE_BLOCK_PARTITION_SIZE;
    int bit_v_index = block_offset;
    cache[index][bit_v_index] = 1;

    // Armazena dado
    for (int i = 0; i < WORD_SIZE; i++) {
        int pos_i = block_offset + TAG_SIZE + 1 + i;
        cache[index][pos_i] = str_to_int(dado[i]);
    }

    // Armazena Tag
    for (int i = 0; i < TAG_SIZE; i++) {
        int pos_i = block_offset + 1 + i;
        cache[index][pos_i] = str_to_int(tag[i]);
    }
}


/*
    Escreve dado na cache, caso a posição já estiver ocupada. A função abaixo
    sobrescreve o valor antigo e guarda o valor antigo na memória.
*/
void escreverDado(int endereco, char *dado, int **cache, int **memoria) {
    // Transformar endereço em binário
    int offset = getOffset(endereco);
    int index = getIndex(endereco);
    string tag = getTag(endereco);

    int bit_v = offset*CACHE_BLOCK_PARTITION_SIZE;
    // se não tem palavra na seção do bloco da cache, faça:
    if(cache[index][bit_v] != 1){
        escreverDadoCache(cache, index, tag, offset, dado);
    } else { // tem dado na cache, passe o dado da cache p/ memória de dados e coloque o novo dado na cache
        /*
            Pega o dado antigo na posição da Cache e escreve na memória.
            Para pegar o endereço certo do dado antigo, temos que recuperar
            ele à partir do indíce do cache, a tag e offset
        */
        int* dado_pra_memoria = getDataFromCache(cache, index, offset);
        // como a tag tá 8 bits deslocada para a direita, multiplicamos por 2 ^ 8
        int tag_int = stoi(getTagFromCache(cache, index, offset), nullptr, 2) * pow(2, INDEX_SIZE + OFFSET_SIZE);
        // como o index tá 2 bits deslocada para a direita, multiplicamos por 2 ^ 2
        int index_value = pow(2, OFFSET_SIZE);
        int endereco_dado_antigo = index_value + offset + tag_int;
        // escreve dado que já ocupava aquela posição da cache na memória
        escreverDadoMemoria(endereco_dado_antigo, dado_pra_memoria, memoria);
        escreverDadoCache(cache, index, tag, offset, dado);
    }
}

/**
    A função abaixo recebe um endereço e a partir dele calcula:
    - o index
    - o offset
    - e a tag
    E a partir desses valores ele checa se o bit de valido daquele bloco de offset
    tá preenchido e se o valor da tag é igual, se sim é um hit, senão um miss
*/
bool lerDado(int endereco, int **cache) {
    int offset = getOffset(endereco);
    int index = getIndex(endereco);
    string tag = getTag(endereco);

    int bit_v = offset*CACHE_BLOCK_PARTITION_SIZE;

    if (cache[index][bit_v] == 1) {
        // Has data
        if (tag == getTagFromCache(cache, index, offset)) {
            // Got a hit
            return true;
        } else {
            // Miss
            return false;
        }
    } else {
        // Miss
        return false;
    }
}
