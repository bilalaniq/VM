#pragma once

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <error.h>
#include <stdbool.h>

/*
errors



*/

#define NoErr 0x00
#define SysHlt 0x01
#define ErrMem 0x02
#define ErrSeqv 0x04

// #define NoArgs {0x00, 0x00}

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

#define segfault(x) Error(x, ErrSeqv)

#define $ax ->c.r.ax
#define $bx ->c.r.bx
#define $cx ->c.r.cx
#define $dx ->c.r.dx
#define $sp ->c.r.sp
#define $ip ->c.r.ip

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

typedef unsigned char Errorcode;

typedef int8 Memory[((int16)(-1))]; // memory will be an array of 8 bits
// In an unsigned 16-bit system, -1 would be 0xFFFF (65,535).
// Safer alternative: typedef int8 Memory[65536];

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
    nop = 0x02, // NOP (No Operation) is an assembly instruction that does nothing except consume a CPU cycle.
    hlt = 0x03
};

typedef enum e_Opcode Opcode;

struct S_instrmap // The IM (instruction map) structure associates opcodes with their sizes.
{
    Opcode o;
    int8 size; // Defines how many bytes the instruction occupies in memory.
};
// When the virtual machine fetches an instruction from memory, it must know how many bytes to read.

typedef struct S_instrmap IM;

typedef int16 Args;

struct S_Instruction
{
    Opcode o;
    Args a[]; // so this will be an array having arguments with an non predefined size ranging from 0->2 bytes
};

typedef struct S_Instruction Instruction;

typedef int8 program;

// while the program at the end will be the instruction but we will consider it an as an byte of string so the pointer
// to an program will be an pointer to an string of bytes so we can later on put out instruction in the program manually

struct S_vm // virtual matchiene struct
{
    CPU c;
    Memory m;
    int16 b; // break line
};

typedef struct S_vm VM;

typedef Memory *Stack;

static IM instrmap[] = {
    // lookup table (an array of IM structures) that maps opcodes to their instruction sizes.

    {mov, 0x03},
    {nop, 0x01},
    {hlt, 0x01},

};

#define IM_size (sizeof(instrmap) / sizeof(IM))

#ifndef prodcode
#define prodcode 1
#endif

#if prodcode
program *exampleprog(VM *);

void Printhex(int8 *, int16, int8);
#endif

void e_mov(VM *, Opcode, Args, Args);

int8 map(Opcode);

VM *VirtualMachine(void);

void execinstr(VM *, program *);

void execute(VM *);

void Error(VM *, Errorcode);

int main(int, char **);
