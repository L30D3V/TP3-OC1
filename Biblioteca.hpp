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

int getOffset(int endereco) {
    bitset<2> bit_offset(endereco);
    // cout << "<< bit_offset: " << bit_offset << '\n';

    return (int)(bit_offset.to_ulong());
}

int getIndex(int endereco) {
    bitset<6> bit_index;
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
    bitset<24> bit_tag;
    bitset<32> bit_endereco(endereco);
    
    for (int i = 0; i < 24; i++) {
        bit_tag[i] = bit_endereco[i+8];
    }

    // cout << "<< bit_endereco: " << bit_endereco << " | bit_tag: " << bit_tag << endl;
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
    // [57]  -> bit_v2; [57-79]   -> tag_2; [80-111]  -> palavra_2
    // [112] -> bit_v3; [113-134] -> tag_3; [135-166] -> palavra_3
    // [167] -> bit_v4; [168-189] -> tag_4; [190-221] -> palavra_3

    int **cache; // cache[64][228]; // 228 = largura = 4 * (1 + 24 + 32)
    cache = (int**) malloc(64 * sizeof(int*));
    for(int i = 0; i < 64; i++)
        cache[i] = (int*) malloc(228 * sizeof(int));

    // Limpar memória cache
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 228; j++) {
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

void escreverDado(int endereco, char *dado, int **cache) {
    // Transformar endereço em binário
    int offset = getOffset(endereco);
    int index = getIndex(endereco);
    string tag = getTag(endereco);

    // cout << "<< endereço: " << endereco << " | dado: " << dado << endl;
    cout << "<< index " << index << " | offset: " << offset << endl;

    int bit_v = offset*57;
    // Verifica se bloco em cache já está sendo utilizado
    if (cache[index][bit_v] != 1) {
        // Marca bit_v como 1
        int bit_v = offset*57;
        cache[index][bit_v] = 1;
        
        // Armazena dado
        for (int i = 0; i < 32; i++) {
            int pos_i = 57*offset + 25 + i;
            cache[index][pos_i] = str_to_int(dado[i]);
        }

        // Armazena Tag
        for (int i = 0; i < 24; i++) {
            int pos_i = 57*offset + 1 + i;
            cache[index][pos_i] = str_to_int(tag[i]);
        }
    } else {
        // Implementa write back na memória e sobrescrita em cache
        cout << ">> Endereco ja ocupado em cache: " << index << endl;
    }
}