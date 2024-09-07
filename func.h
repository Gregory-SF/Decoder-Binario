#ifndef FUNC_H
#define FUNC_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>  

#define NUM_REGISTERS 8
uint16_t registers[NUM_REGISTERS] = {0};

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
}

static void sub(Instruction comando) {
    registers[comando.reg_dst] = registers[comando.reg_1] - registers[comando.reg_2];
}

static void mul(Instruction comando) {
    registers[comando.reg_dst] = registers[comando.reg_1] * registers[comando.reg_2];
}

static void divi(Instruction comando) {
    if (registers[comando.reg_2] != 0) {
        registers[comando.reg_dst] = registers[comando.reg_1] / registers[comando.reg_2];
    } else {
        printf("Erro: Divisão por zero.\n");
    }
}

static void cmp_equal(Instruction comando) {
    registers[comando.reg_dst] = (registers[comando.reg_1] == registers[comando.reg_2]) ? 1 : 0;
}

static void cmp_neq(Instruction comando) {
    registers[comando.reg_dst] = (registers[comando.reg_1] != registers[comando.reg_2]) ? 1 : 0;
}

void load(Instruction comando) {
    registers[comando.reg_dst] = registers[comando.reg_1];
    //registers[reg_dst] = &registers[reg1];
    printf("regis[reg]: %u\n",registers[comando.reg_dst]);
    printf("regis[reg1]: %u\n",registers[comando.reg_1]);
    //printf("&pont[reg_dst]: %u\n",&registers[reg1]);
}

void store(Instruction comando) {
    registers[comando.reg_1] = registers[comando.reg_2];
    //registers[reg_2] = &registers[reg_1];
    printf("regis[reg2]: %u\n",registers[comando.reg_2]);
    printf("regis[reg1]: %u\n",registers[comando.reg_1]);
    //printf("&pont[reg2]: %u\n",&registers[reg_1]);
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
    printf("regis[reg]: %u\n",registers[comando->reg_dst]);

}

static void syscall() {
    if (registers[0]== 0) {
        printf("Encerrando o programa.\n");
        exit(0); 
    }
    else if (registers[0] == 2){
        printf("Registradores: %u\n",registers[1]);
        printf("Valor do registrador: %u\n",registers[registers[1]]);
    }
    else {
        printf("Serviço do sistema não suportado ou não implementado: %u\n", registers[0]);
    }
}

void execute_instructionr(Instruction comando) {
    switch (comando.opcode) {
        case 0:
            add(comando);
            printf("Valor em registers[2]: %u\n", registers[comando.reg_dst]);
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
            store(comando);  // reg1 é a fonte para store
            break;
        case 63:
            syscall();
            break;
        default:
            printf("Erro: Opcode desconhecido.\n");
            break;
        }
    }
void execute_instructioni(Instruction *comando) {
    switch (comando->opcode){
        case 0:
            printf("Imediato = %d, Linha = %d\n",comando->imediato,comando->linha);
            jump(comando);
            printf("Imediato = %d, Linha = %d\n",comando->imediato,comando->linha);
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
}
#endif 