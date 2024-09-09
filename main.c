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
    int g;
    load_binary_to_memory(argv[1], memory_pointer,MEMORY_SIZE);
    for(g = 0; g < MEMORY_SIZE; g++){
        if(g == 10){
            break;
        }
        printf("memory[%d]: %d\n",g,memory[g]);
    }
    int cond = 1;
    Instruction comando;
    int i = 1;
    while(cond){
            comando.linha = i;
            int format = extract_bits(memory[i], 15,1);
            printf("Linha %d\n",i);
            printf("format: %d, ",format);
            if(format == 0){
                comando.opcode = extract_bits(memory[i], 9, 6);
                comando.reg_dst = extract_bits(memory[i], 6, 3);
                comando.reg_1 = extract_bits(memory[i], 3, 3);
                comando.reg_2 = extract_bits(memory[i], 0, 3);
                printf("opcode: %d, reg_dst: %d, reg_1: %d, reg_2: %d\n",comando.opcode,comando.reg_dst,comando.reg_1,comando.reg_2);
                instruction_r(comando);
            }
            else {
                comando.opcode = extract_bits(memory[i], 13, 2);
                comando.reg_dst = extract_bits(memory[i], 10, 3);
                comando.imediato = extract_bits(memory[i], 0, 10);
                printf("opcode: %d, reg_dst: %d, imediato: %d\n",comando.opcode,comando.reg_dst,comando.imediato);
                instruction_i(&comando);
                i = comando.linha;       
                }
            printf("--------------------\n");
            i++;
        }   
}
