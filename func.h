#ifndef FUNC_H
#define FUNC_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>  

#define NUM_REGISTERS 8
#define MEMORY_SIZE 1024

uint16_t registers[NUM_REGISTERS] = {0};
extern uint16_t memory[];

typedef struct Instruction {
    uint16_t format;
    uint16_t opcode;
    uint16_t reg_dst;
    uint16_t reg_1;
    uint16_t reg_2;
    uint16_t imediato;
    uint16_t linha;
}Instruction;

static void printar_registradores(){
    int i;
    for(i = 0; i < NUM_REGISTERS; i++){
        printf("Registrador [%d]: %d\n", i, registers[i]);
    }
}

static void printar_memory(){ 
    int g;
    for(g = 0; g < MEMORY_SIZE; g++){
        if(g == 20){
            break;
        }
        printf("memory[%d]: %d\n",g,memory[g]);
    }
}

static void printar_Comando(Instruction comando){
    printf("format: %d, ",comando.format);
    if(comando.format == 0){
        printf("opcode: %d, reg_dst: %d, reg_1: %d, reg_2: %d\n",comando.opcode,comando.reg_dst,comando.reg_1,comando.reg_2);
    }
    else {
        printf("opcode: %d, reg_dst: %d, imediato: %d\n",comando.opcode,comando.reg_dst,comando.imediato);
    }
}           

static uint16_t buscar_instrucao(int pc){
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
    printf("Valor em register[%d]: %d, registers[%d]: %d, registers[%d]: %d\n", comando->reg_1, registers[comando->reg_1], comando->reg_2, registers[comando->reg_2], comando->reg_dst, registers[comando->reg_dst]);
}

static void sub(Instruction *comando) {
    registers[comando->reg_dst] = registers[comando->reg_1] - registers[comando->reg_2];
    printf("Valor em register[%d]: %d, registers[%d]: %d, registers[%d]: %d\n", comando->reg_1, registers[comando->reg_1], comando->reg_2, registers[comando->reg_2], comando->reg_dst, registers[comando->reg_dst]);
}

static void mul(Instruction *comando) {
    registers[comando->reg_dst] = registers[comando->reg_1] * registers[comando->reg_2];
    printf("Valor em register[%d]: %d, registers[%d]: %d, registers[%d]: %d\n", comando->reg_1, registers[comando->reg_1], comando->reg_2, registers[comando->reg_2], comando->reg_dst, registers[comando->reg_dst]);
}

static void divi(Instruction *comando) {
    if (registers[comando->reg_2] != 0) {
        registers[comando->reg_dst] = registers[comando->reg_1] / registers[comando->reg_2];
    printf("Valor em register[%d]: %d, registers[%d]: %d, registers[%d]: %d\n", comando->reg_1, registers[comando->reg_1], comando->reg_2, registers[comando->reg_2], comando->reg_dst, registers[comando->reg_dst]);
    } else {
        printf("Erro: Divisão por zero.\n");
    }
}

static void cmp_equal(Instruction *comando) {
    registers[comando->reg_dst] = (registers[comando->reg_1] == registers[comando->reg_2]) ? 1 : 0;
    printf("Valor em registers[%d]: %d, valor em registers[%d]: %d\n", comando->reg_1, registers[comando->reg_1], comando->reg_2, registers[comando->reg_2]);
    printf("Valor em registers[%d]: %d\n", comando->reg_dst, registers[comando->reg_dst]);
}

static void cmp_neq(Instruction *comando) {
    registers[comando->reg_dst] = (registers[comando->reg_1] != registers[comando->reg_2]) ? 1 : 0;
    printf("Valor em registers[%d]: %d, valor em registers[%d]: %d\n", comando->reg_1, registers[comando->reg_1], comando->reg_2, registers[comando->reg_2]);
    printf("Valor em registers[%d]: %d\n", comando->reg_dst, registers[comando->reg_dst]);
}

static void load(Instruction *comando) {
    registers[comando->reg_dst] = memory[registers[comando->reg_1]];
    printf("Valor em registers[%d]: %d, valor em registers[%d]: %d, valor na memória: %u\n", comando->reg_1, registers[comando->reg_1], comando->reg_dst, registers[comando->reg_dst],memory[registers[comando->reg_1]]);
}

static void store(Instruction *comando) {
    memory[registers[comando->reg_1]] = registers[comando->reg_2];
    printf("Valor em regiters[%d]: %d, valor em registers[%d]: %d, valor na memória: %u\n", comando->reg_1, registers[comando->reg_1], comando->reg_2, registers[comando->reg_2], memory[registers[comando->reg_1]]);
    int i;
    for(i = 0; i <= registers[comando->reg_1]; i++){
    printf("memory[%d]: %d\n",i,memory[i]);
    }
}

static void jump(Instruction *comando) {
    comando->linha = comando->imediato-1;
}

static void jump_cond(Instruction *comando) {
    if (registers[comando->reg_dst] == 1) {
        comando->linha = comando->imediato-1;
    }
}

static void mov(Instruction *comando) {
    registers[comando->reg_dst] = comando->imediato;
    printf("Valor em registers[%d]: %d\n", comando->reg_dst, registers[comando->reg_dst]);
}

static void syscall() {
    printar_registradores();
    switch (registers[0]){
    case 0:
        printar_memory();
        printf("Encerrando o programa.\n");
        exit(0); 
    case 2:
        printf("\n");
        break;
    case 3:
        printf("Valor em no registers[1]: %d\n",registers[1]);
        break;
    default:
        printf("Serviço do sistema não suportado ou não implementado: %u\n", registers[0]);
        break;
    }
}

static void instruction_r(Instruction *comando) {
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
}

static void instruction_i(Instruction *comando) {
    switch (comando->opcode){
        case 0:
            printf("Imediato = %d, Linha = %d\n",comando->imediato,comando->linha);
            jump(comando);
            printf("Imediato = %d, Linha = %d\n",comando->imediato,comando->linha);
            break;
        case 1:
            printf("Valor em registers[dst] = %d, Imediato = %d, Linha = %d\n", comando->reg_dst, comando->imediato, comando->linha);
            jump_cond(comando);
            printf("Valor em registers[dst] = %d, Imediato = %d, Linha = %d\n", comando->reg_dst, comando->imediato, comando->linha);
            break;
        case 3:
            mov(comando);
            break;
        default:
            printf("Erro: Opcode desconhecido.\n");
            break;
    }
}

void executar_instrucao(Instruction *comando){
    switch (comando->format) {
        case 0:
            instruction_r(comando);
            break;
        case 1:
            instruction_i(comando);
            break;
    }
}

#endif 