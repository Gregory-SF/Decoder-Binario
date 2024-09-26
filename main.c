#include <stdio.h>
#include <stdint.h>  // Para tipos de dados de tamanho fixo
#include <stdlib.h>  // Para exit()
#include <locale.h>
#include <assert.h>

#include "lib.h"
#include "func.h"

uint16_t memory[MEMORY_SIZE];
void *memory_pointer = memory;

int main (int argc, char **argv){
	if (argc != 2) {
		printf("usage: %s [bin_name]\n", argv[0]);
		exit(1);
	}
    load_binary_to_memory(argv[1], memory_pointer,MEMORY_SIZE);
    printar_memory();
    int cond = 1;
    Instruction comando;
    int pc = 1;
    while(cond){
            uint16_t instrucao = buscar_instrucao(pc);
            comando.linha = pc;
            decodificar_instrucao(instrucao, &comando);
            printf("Linha %d\n",pc);
            printar_Comando(comando);
            if(comando.format == 0){
                instruction_r(comando);
            }
            else {
                instruction_i(&comando);
                pc = comando.linha;       
            }
            printf("--------------------\n");
            pc++;
        }   
}
