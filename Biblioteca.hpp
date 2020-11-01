#include <iostream>
#include <bitset>
using namespace std;
/* 
######### Começo funções auxiliares
*/
// Converte uma string em inteiro
int str_to_int(char string){
    if(string == '1')
        return 1;
    else
        return 0;
}
// Converte uma string (binário) em inteiro
char bin_to_int(string bin){
    int offset;
    // Pega as posições 30 e 31 e calcula o offset da palavra
    if(bin[30] == '0'){
        bin[31] == '0' ? offset = 0 : offset = 1;
    }else{
        bin[31] == '0' ? offset = 2 : offset = 3;
    }
    return offset;
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

    string indice_binario;
    for(int i = 0; i < 64; i++){
        cache[i][6] = 0; // zera o bit de validade
        indice_binario = bitset<6>(i).to_string(); // converte p/ binário

        // escreve os bits de índice na cache
        for(int bin_index = 0; bin_index <= 5; bin_index++)
            cache[i][bin_index] = str_to_int(indice_binario[bin_index]);

        // ### Temporário, Zerando as outras posições da cache
        // ### p/ visualizar modificações na cache
        // ### senão aparece lixo nas posições da cache
        for(int j = 7; j < 162; j++)
            cache[i][j] = 0;
        // ### Temporário, para visualizar modificações na cache
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
    int localizacao, offset;
    localizacao = endereco % 64;
    string endereco_extendido = bitset<32>(endereco).to_string(); // converte o endereço p/ binário e extende p/ 32 bits
    // Offset vai ser os 2 últimos bits do endereço
    offset = bin_to_int(endereco_extendido);
    // cout << endereco_extendido<< ":::";
    // cout<<"offset: "<<offset<<endl;
    // Checar se o bloco está vazio
    if(cache[localizacao][6] == 0){ // se o bloco está vazio
        cache[localizacao][6] = 1; // coloca o bit válido
        
        // escreve os bits da tag na cache
        for(int bin_index = 9, i = 0; bin_index <= 32; i++, bin_index++)
            cache[localizacao][bin_index] = str_to_int(endereco_extendido[i]);

        // escreve os bits do offset na cache
        for(int bin_index = 7, i = 30; bin_index <= 8; i++, bin_index++)
            cache[localizacao][bin_index] = str_to_int(endereco_extendido[i]);

        // armazena o dado de acordo com o offset
        // se offset = 0, armazena na cache de 33 a 65, se offset = 1, armazena na cache de 66 a 97, etc
        for(int j = 33*(offset+1), i = 0; i <= 31; j++, i++)
            cache[localizacao][j] = str_to_int(dado[i]);
    }else{
        // Se o bloco está cheio, faça...
        // Pode ocorrer q o dado buscado seja armazenado em uma posição na cache que já tenha outro dado lá,
        // então teria q atualizar a cache
    }

    // #### Testando
    // for(int j = 33*(offset+1); j < (33*(offset+1)+32); j++)
    //     cout << cache[localizacao][j];
    // cout << endl;
}

// Operações de leitura enviam um endereço que desejam acessar,
// esse endereço é passado para a cache que retorna o dado caso um hit ocorra,
// ou busca um bloco na memória de dados caso um miss ocorra.

/*
Um dado na cache, também mapearia para a memória.
Alguns dados da memória vão mapear para o mesmo endereço na cache
*/

// lerDado();