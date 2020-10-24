// Memória cache - 64 blocos
// - 16 bytes p/ bloco - 128 bits
// - 4 palavras de 32 bits p/ bloco --- ou seja, em cada bloco cabem 4 palavras - precisamos criar um offset então
//  matriz ~~64x128~~
void inicializaCache(){
    /* Inicializar o "bit válido" com 0s para indicar
        que a posição está vazia - pode ter lixo lá  */
    // acho que primeiro endereça o bloco, e precisa-se de 8 bits para isso
    // e depois    
}


// Memória de dados - 1024 palavras de 32 bits
//  matriz 1024x32
void inicializaMemoria(){

}