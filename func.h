#ifndef FUNC_H
#define FUNC_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>  

#define NUM_REGISTERS 8
#define MEMORY_SIZE 1024

uint16_t registers[NUM_REGISTERS] = {0};
extern uint16_t memory[];
extern int pc;

typedef struct Instruction {
    uint16_t format;
    uint16_t opcode;
    uint16_t reg_dst;
    uint16_t reg_1;
    uint16_t reg_2;
    uint16_t imediato;
}Instruction;

static void printar_registradores(){
    printf("\n");
    int i;
    for(i = 0; i < NUM_REGISTERS; i++){
        printf("Registrador [%d]: %d\n", i, registers[i]);
    }
    printf("\n");
}

static void printar_memory(){ 
    printf("\n");
    int index;
    for(index = 0; index <= 100; index++){
        //printf("memory[%d]: %d\n",index,memory[index]);
        printf("%d ",memory[index]);
    }
    printf("\n");
}


static uint16_t buscar_instrucao(){
    return memory[pc];
}

static void decodificar_instrucao(uint16_t instrucao, Instruction *comando){
    comando->format = extract_bits(instrucao, 15,1);
    switch (comando->format) {
        case 0:
            comando->opcode = extract_bits(instrucao, 9, 6);
            comando->reg_dst = extract_bits(instrucao, 6, 3);
            comando->reg_1 = extract_bits(instrucao, 3, 3);
            comando->reg_2 = extract_bits(instrucao, 0, 3);
            break;
        case 1:
            comando->opcode = extract_bits(instrucao, 13, 2);
            comando->reg_dst = extract_bits(instrucao, 10, 3);
            comando->imediato = extract_bits(instrucao, 0, 10);
            break;
    }
}

static void add(Instruction *comando) {
    registers[comando->reg_dst] = registers[comando->reg_1] + registers[comando->reg_2];
    // printf("Valor em register[%d]: %d, registers[%d]: %d, registers[%d]: %d\n", comando->reg_1, registers[comando->reg_1], comando->reg_2, registers[comando->reg_2], comando->reg_dst, registers[comando->reg_dst]);
}

static void sub(Instruction *comando) {
    registers[comando->reg_dst] = registers[comando->reg_1] - registers[comando->reg_2];
    // printf("Valor em register[%d]: %d, registers[%d]: %d, registers[%d]: %d\n", comando->reg_1, registers[comando->reg_1], comando->reg_2, registers[comando->reg_2], comando->reg_dst, registers[comando->reg_dst]);
}

static void mul(Instruction *comando) {
    registers[comando->reg_dst] = registers[comando->reg_1] * registers[comando->reg_2];
    // printf("Valor em register[%d]: %d, registers[%d]: %d, registers[%d]: %d\n", comando->reg_1, registers[comando->reg_1], comando->reg_2, registers[comando->reg_2], comando->reg_dst, registers[comando->reg_dst]);
}

static void divi(Instruction *comando) {
    if (registers[comando->reg_2] != 0) {
        registers[comando->reg_dst] = registers[comando->reg_1] / registers[comando->reg_2];
    // printf("Valor em register[%d]: %d, registers[%d]: %d, registers[%d]: %d\n", comando->reg_1, registers[comando->reg_1], comando->reg_2, registers[comando->reg_2], comando->reg_dst, registers[comando->reg_dst]);
    } else {
        printf("Erro: Divisão por zero.\n");
    }
}

static void cmp_equal(Instruction *comando) {
    registers[comando->reg_dst] = (registers[comando->reg_1] == registers[comando->reg_2]) ? 1 : 0;
    // printf("Valor em registers[%d]: %d, valor em registers[%d]: %d\n", comando->reg_1, registers[comando->reg_1], comando->reg_2, registers[comando->reg_2]);
    // printf("Valor em registers[%d]: %d\n", comando->reg_dst, registers[comando->reg_dst]);
}

static void cmp_neq(Instruction *comando) {
    registers[comando->reg_dst] = (registers[comando->reg_1] != registers[comando->reg_2]) ? 1 : 0;
    // printf("Valor em registers[%d]: %d, valor em registers[%d]: %d\n", comando->reg_1, registers[comando->reg_1], comando->reg_2, registers[comando->reg_2]);
    // printf("Valor em registers[%d]: %d\n", comando->reg_dst, registers[comando->reg_dst]);
}

static void load(Instruction *comando) {
    registers[comando->reg_dst] = memory[registers[comando->reg_1]];
    // printf("Valor em registers[%d]: %d, valor em registers[%d]: %d, valor na memória: %u\n", comando->reg_1, registers[comando->reg_1], comando->reg_dst, registers[comando->reg_dst],memory[registers[comando->reg_1]]);
}

static void store(Instruction *comando) {
    memory[registers[comando->reg_1]] = registers[comando->reg_2];
    // printf("Valor em regiters[%d]: %d, valor em registers[%d]: %d, valor na memória: %u\n", comando->reg_1, registers[comando->reg_1], comando->reg_2, registers[comando->reg_2], memory[registers[comando->reg_1]]);
}

static void jump(Instruction *comando) {
    pc = comando->imediato-1;
}

static void jump_cond(Instruction *comando) {
    if (registers[comando->reg_dst] == 1) {
        pc = comando->imediato-1;
    }
}

static void mov(Instruction *comando) {
    registers[comando->reg_dst] = comando->imediato;
    // printf("Valor em registers[%d]: %d\n", comando->reg_dst, registers[comando->reg_dst]);
}

static void syscall() {
    switch (registers[0]){
        case 0:
            printar_registradores();
            printar_memory();
            printf("Encerrando o programa.\n");
            exit(0); 
        case 1:
            int i = registers[1];
            while(memory[i] != 0){
                printf("%c", (char)memory[i]);
                i++;
            }
            break;
        case 2:
            printf("\n");
            break;
        case 3:
            //printf("Valor em no registers[1]: %d\n",registers[1]);
            printf("%d",registers[1]);
            break;
        default:
            printf("Serviço do sistema não suportado ou não implementado: %u\n", registers[0]);
            break;
        }
}

static void executar_instrucao(Instruction *comando){
    switch (comando->format) {
        case 0: // Instrução do tipo R
            switch (comando->opcode) {
                case 0:
                    add(comando);
                    break;
                case 1:
                    sub(comando);
                    break;
                case 2: 
                    mul(comando);
                    break;
                case 3:
                    divi(comando);
                    break;
                case 4:
                    cmp_equal(comando);
                    break;
                case 5:
                    cmp_neq(comando);
                    break;
                case 15:
                    load(comando);
                    break;
                case 16:
                    store(comando); 
                    break;
                case 63:
                    syscall();
                    break;
                default:
                    printf("Erro: Opcode desconhecido.\n");
                    break;
                }
            break;
        case 1: // Instrução do tipo I
            switch (comando->opcode){
                case 0:
                    jump(comando);
                    break;
                case 1:
                    jump_cond(comando);
                    break;
                case 3:
                    mov(comando);
                    break;
                default:
                    printf("Erro: Opcode desconhecido.\n");
                    break;
            }
            break;
    }
}

#endif 