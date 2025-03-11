#pragma once

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <error.h>
#include <stdbool.h>

/*
errors



*/

#define ErrMem 0x01

/* Limtations of this vm

16 bit vm
      AX
      BX
      CX
      DX
      SP
      IP

65536 different memory addresses that mean that our memory will be 65KB

(Serial COM Port)
(floppy drive)
*/

typedef unsigned char int8;           // 8 bit
typedef unsigned short int int16;     // 16 bit
typedef unsigned int int32;           // 32 bit
typedef unsigned long long int int64; // 64 bit

#define $1 (int8 *) // Pointer to int8 (unsigned char)
#define $ (int16)   // Cast to int16 (unsigned short int)
#define $2 (int32)  // Cast to int32 (unsigned int)
#define $4 (int64)  // Cast to int64 (unsigned long long int)
#define $c (char *) // Cast to char pointer
#define $i (int)    // Cast to int

typedef unsigned short int Reg; // because we are dealing with 16 bit regiters

struct S_registers
{
    Reg ax;
    Reg bx;
    Reg cx;
    Reg dx;
    Reg sp;
    Reg ip;
};
typedef struct S_registers REGISTERS;

struct S_cpu
{
    REGISTERS r;
};

typedef struct S_cpu CPU;

typedef int8 STACK[((unsigned int)(-1))]; // stack will be an array of 8 bits
// In an unsigned 16-bit system, -1 would be 0xFFFF (65,535).
// Safer alternative: typedef int8 STACK[65536];

// note : When you cast -1 to an unsigned int, it results in the maximum possible value for that type.

/*
                   opcode representasion
mov ax 0x05    //  (0x01 OR 0x02)
                   0000 0011 = mov
                   0000 0000
                   0000 0101 = 0x05
*/

enum e_Opcode
{
    mov = 0x01,
    nop = 0x02 // NOP (No Operation) is an assembly instruction that does nothing except consume a CPU cycle.
};

typedef enum e_Opcode Opcode;

struct S_instrmap // The IM (instruction map) structure associates opcodes with their sizes.
{
    Opcode o;
    int8 size; // Defines how many bytes the instruction occupies in memory.
};
// When the virtual machine fetches an instruction from memory, it must know how many bytes to read.

typedef struct S_instrmap IM;

typedef int8 Args;

struct S_Instruction
{
    Opcode o;
    Args a[]; // so this will be an array having arguments with an non predefined size ranging from 0->2 bytes
};

typedef struct S_Instruction Instruction;

typedef Instruction program;

struct S_vm // virtual matchiene struct
{
    CPU c;
    STACK s;
    program *p;
};

typedef struct S_vm VM;

static IM instrmap[] = { // lookup table (an array of IM structures) that maps opcodes to their instruction sizes.

    {mov, 0x03},
    {nop, 0x01}

};

VM *VirtualMachine(program * , int16);

int main(int, char **);
