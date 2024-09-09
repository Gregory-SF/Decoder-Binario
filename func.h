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
    int opcode;
    int reg_dst;
    int reg_1;
    int reg_2;
    int imediato;
    int linha;
}Instruction;

static void add(Instruction comando) {
    registers[comando.reg_dst] = registers[comando.reg_1] + registers[comando.reg_2];
    printf("Valor em register[%d]: %d, registers[%d]: %d, registers[%d]: %d\n", comando.reg_1, registers[comando.reg_1], comando.reg_2, registers[comando.reg_2], comando.reg_dst, registers[comando.reg_dst]);
}

static void sub(Instruction comando) {
    registers[comando.reg_dst] = registers[comando.reg_1] - registers[comando.reg_2];
    printf("Valor em register[%d]: %d, registers[%d]: %d, registers[%d]: %d\n", comando.reg_1, registers[comando.reg_1], comando.reg_2, registers[comando.reg_2], comando.reg_dst, registers[comando.reg_dst]);
}

static void mul(Instruction comando) {
    registers[comando.reg_dst] = registers[comando.reg_1] * registers[comando.reg_2];
    printf("Valor em register[%d]: %d, registers[%d]: %d, registers[%d]: %d\n", comando.reg_1, registers[comando.reg_1], comando.reg_2, registers[comando.reg_2], comando.reg_dst, registers[comando.reg_dst]);
}

static void divi(Instruction comando) {
    if (registers[comando.reg_2] != 0) {
        registers[comando.reg_dst] = registers[comando.reg_1] / registers[comando.reg_2];
    printf("Valor em register[%d]: %d, registers[%d]: %d, registers[%d]: %d\n", comando.reg_1, registers[comando.reg_1], comando.reg_2, registers[comando.reg_2], comando.reg_dst, registers[comando.reg_dst]);
    } else {
        printf("Erro: Divisão por zero.\n");
    }
}

static void cmp_equal(Instruction comando) {
    registers[comando.reg_dst] = (registers[comando.reg_1] == registers[comando.reg_2]) ? 1 : 0;
    printf("Valor em registers[%d]: %d, valor em registers[%d]: %d\n", comando.reg_1, registers[comando.reg_1], comando.reg_2, registers[comando.reg_2]);
    printf("Valor em registers[%d]: %d\n", comando.reg_dst, registers[comando.reg_dst]);
}

static void cmp_neq(Instruction comando) {
    registers[comando.reg_dst] = (registers[comando.reg_1] != registers[comando.reg_2]) ? 1 : 0;
    printf("Valor em registers[%d]: %d, valor em registers[%d]: %d\n", comando.reg_1, registers[comando.reg_1], comando.reg_2, registers[comando.reg_2]);
    printf("Valor em registers[%d]: %d\n", comando.reg_dst, registers[comando.reg_dst]);
}

static void load(Instruction comando) {
    registers[comando.reg_dst] = memory[registers[comando.reg_1]];
    printf("Valor em registers[%d]: %d, valor em registers[%d]: %d, valor na memória: %u\n", comando.reg_1, registers[comando.reg_1], comando.reg_dst, registers[comando.reg_dst],memory[registers[comando.reg_1]]);
}

static void store(Instruction comando) {
    memory[registers[comando.reg_1]] = registers[comando.reg_2];
    printf("Valor em regiters[%d]: %d, valor em registers[%d]: %d, valor na memória: %u\n", comando.reg_1, registers[comando.reg_1], comando.reg_2, registers[comando.reg_2], memory[registers[comando.reg_1]]);
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

static void printar_registradores(){
    int i;
    for(i = 0; i < NUM_REGISTERS; i++){
        printf("Registrador [%d]: %d\n", i, registers[i]);
    }
}

static void syscall() {
    printar_registradores();
    switch (registers[0]){
    case 0:
        int i;
        for(i = 0; i < MEMORY_SIZE; i++){
        if(i == 10){
            break;
        }
        printf("memory[%d]: %d\n",i,memory[i]);
    }
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

void instruction_r(Instruction comando) {
    switch (comando.opcode) {
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
void instruction_i(Instruction *comando) {
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
#endif 