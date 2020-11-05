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

int getOffset(int endereco) {
    bitset<OFFSET_SIZE> bit_offset(endereco);
    // cout << "<< bit_offset: " << bit_offset << '\n';

    return (int)(bit_offset.to_ulong());
}

int getIndex(int endereco) {
    bitset<INDEX_SIZE> bit_index;
    bitset<8> bit_endereco(endereco);

    bit_index[0] = bit_endereco[2];
    bit_index[1] = bit_endereco[3];
    bit_index[2] = bit_endereco[4];
    bit_index[3] = bit_endereco[5];
    bit_index[4] = bit_endereco[6];
    bit_index[5] = bit_endereco[7];

    // cout << "<< bit_endereco: " << bit_endereco << " | bit_index: " << bit_index << '\n';

    return (int)(bit_index.to_ulong());
}

string getTag(int endereco) {
    bitset<TAG_SIZE> bit_tag;
    bitset<WORD_SIZE> bit_endereco(endereco);

    for (int i = 0; i < TAG_SIZE; i++) {
        bit_tag[i] = bit_endereco[i+8];
    }

    cout << "<< bit_endereco: " << bit_endereco << " | bit_tag: " << bit_tag << endl;
    return bit_tag.to_string();
}

int* getDataFromCache(int** cache, int index, int offset) {
    int pos_i = CACHE_BLOCK_PARTITION_SIZE*offset + TAG_SIZE + BIT_V_SIZE;
    int pos_end = pos_i + WORD_SIZE;
    int data[WORD_SIZE];

    int i = 0;
    while (pos_i <= pos_end) {
        // cout << "i: " << i << " | pos_i: " << pos_i << " | bit_tag: " << bit_tag[i] << endl;

        data[WORD_SIZE - 1 - i] = cache[index][pos_i];
        i++;
        pos_i++;
    }

    return data;
}

string getTagFromCache(int **cache, int index, int offset) {
    int pos_i = offset*CACHE_BLOCK_PARTITION_SIZE + BIT_V_SIZE;
    int pos_end = pos_i + TAG_SIZE;
    bitset<TAG_SIZE> bit_tag;

    int i = 0;
    while (pos_i <= pos_end) {
        // cout << "i: " << i << " | pos_i: " << pos_i << " | bit_tag: " << bit_tag[i] << endl;

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

// Memória de dados - 1024 palavras de 32 bits
// matriz 1024x32
int** inicializaMemoria(){
    //  A VERIFICAR SE IMPLEMENTAÇÃO ESTÁ CORRETA
    int **memoria; // memoria[1024][32];
    memoria = (int**) malloc(MEMORY_NUM_BLOCKS * sizeof(int*));
    for(int i = 0; i < MEMORY_NUM_BLOCKS; i++)
        memoria[i] = (int*) malloc(WORD_SIZE * sizeof(int));

    return memoria;
}

void printLinhaCache(int** cache, int index) {
    for (int j = 0; j < CACHE_NUM_BLOCKS; j++) {
        cout << cache[index][j];
        if (j == 0 || j == 24 || j == 56 || j == 57 || j == 81 || j == 113 || j == 114 || j == 138 || j == 170 || j == 171 || j == 195)
            cout << "|";
    }
    cout << endl;
}

void escreverDadoMemoria(int endereco, int* dado, int** memoria) {
    for(int i = 0; i < WORD_SIZE; i++) {
        memoria[endereco][i] = dado[i];
    }
}

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

void escreverDado(int endereco, char *dado, int **cache, int **memoria) {
    // Transformar endereço em binário
    int offset = getOffset(endereco);
    int index = getIndex(endereco);
    string tag = getTag(endereco);

    // cout << "<< endereço: " << endereco << " | dado: " << dado << endl;
    cout << "<< index " << index << " | offset: " << offset << endl;

    int bit_v = offset*CACHE_BLOCK_PARTITION_SIZE;
    // se não tem palavra na seção do bloco da cache, faça:
    if(cache[index][bit_v] != 1){
        escreverDadoCache(cache, index, tag, offset, dado);
    } else { // tem dado na cache, passe o dado da cache p/ memória de dados e coloque o novo dado na cache
        cout << "Antes"<< endl;
        printLinhaCache(cache, index);
        int* dado_pra_memoria = getDataFromCache(cache, index, offset);
        int tag_int = stoi(getTagFromCache(cache, index, offset), nullptr, 2) * pow(2, INDEX_SIZE + OFFSET_SIZE);
        int index_value = pow(2, OFFSET_SIZE);
        int endereco_dado_antigo = index_value + offset + tag_int;
        cout << "tag_int: " << tag_int << endl;
        cout << "tag binary value: " << getTagFromCache(cache, index, offset) << endl;
        cout << "offset: " << offset << endl;
        cout << "index: " << index << endl;
        cout << "endereco_dado_antigo: " << endereco_dado_antigo << endl;
        escreverDadoMemoria(endereco_dado_antigo, dado_pra_memoria, memoria);
        escreverDadoCache(cache, index, tag, offset, dado);
        cout << "Depoirs "<< endl;
        printLinhaCache(cache, index);
    }
}

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
