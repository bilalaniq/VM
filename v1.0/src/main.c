#include "../include/utils.h"

void e_mov(VM *vm, Opcode op, Args a1, Args a2)
{
    vm $ax = (Reg)a1;

    return;
}

void execinstr(VM *vm, program *p)
{
    Args a1, a2;

    int16 size;

    a1 = a2 = 0;

    size = map(*p);

    switch (size)
    {
    case 1:
        break;

    case 2:
        a1 = *(p + 1);
        printf("DEBUG: a1 = 0x%02X", a1);
        break;

    case 3:

        a1 = *(p + 1);
        a2 = *(p + 3);
        printf("DEBUG: a1 = 0x%02X, a2 = 0x%02X\n", a1, a2);

        break;

    default:
        segfault(vm);
        break;
    }

    switch (*p)
    {
    case mov:
        e_mov(vm, *p, a1, a2);
        break;

    case nop:
        break;

    case hlt:

        Error(vm, SysHlt);
        break;

    default:
        break;
    }

    return;
}

void execute(VM *vm)
{
    int16 breakaddr;
    program *pp;
    int16 size;

    Instruction *ip; // note that thi is not the register ip

    assert(vm && *vm->m);
    size = 0;
    // breakaddr = (int32)(vm->m + vm->b);
    breakaddr = (int16)(vm->m + vm->b);

    pp = (program *)vm->m;

    // in our example program we have
    /*
    instr1 arg1 instr2 instr3
    mov ax, 0x05; nop; hlt;
    0x01 0x00 0x05  ; mov
    0x02  ; nop
    0x03  ; hlt
    */

    while (*pp != (Opcode)hlt)
    {

        vm $ip += size;

        pp += size;
        ip->o = *pp;

        if ((int16)pp > breakaddr)
        {
            segfault(vm);
        }
        size = map(ip->o);

        execinstr(vm, pp);
    }

    return;
}

void Error(VM *vm, Errorcode e)
{

    int8 exitcode;
    exitcode = -1;

    switch (e)
    {
    case ErrSeqv:
        fprintf(stderr, "%s\n", "VM segmentation fault");
        break;

    case SysHlt:

        fprintf(stderr, "%s\n", "system halted");
        printf("ax = %0.04hx\n", $i vm $ax);
        exitcode = 0;
        break;

    default:
        break;
    }

    if (vm)
        free(vm);

    exit($i exitcode);
}

VM *VirtualMachine()
{
    VM *p;
    int16 size;
    // program *pp;

    size = $ sizeof(VM);
    p = (VM *)malloc($i size);

    if (!p)
    {
        // errno = ErrMem;

        return (VM *)0;
    }

    memset(p, 0, size);

    return p;
}

int8 map(Opcode o)
{
    int8 n, ret;
    IM *p;
    ret = 0;

    for (n = IM_size, p = instrmap; n; n--, p++)
    {
        if (p->o == o)
        {
            ret = p->size;
            // printf("Mapped Opcode: 0x%02X -> Size: %d\n", o, ret);
            return ret;
        }
    }

    // printf("Unknown Opcode: 0x%02X\n", o); // Debugging print
    return 0; // Invalid opcode should return 0
}

#if prodcode // production code

program *exampleprog(VM *vm)
{
    program *p;
    Instruction *i1, *i2, *i3;
    Args a1 = 0;
    int16 S1, S2, S3, SA1;

    S1 = map(mov);
    S2 = map(nop);
    S3 = map(hlt);

    i1 = (Instruction *)malloc($i S1);
    i2 = (Instruction *)malloc(S2);
    i3 = (Instruction *)malloc(S3);

    assert(i1 && i2 && i3);

    memset($1 i1, 0, S1);
    memset($1 i2, 0, S2);
    memset($1 i3, 0, S3);

    i1->o = mov;
    SA1 = (S1 - 1);

    a1 = 0x0005;

    /*
    0000 0001 mov eax
    0000 0000
    0000 0005 mov eax , 0x05

    */

    p = vm->m;
    memcpy($1 p, $1 i1, 1);
    p++;
    memcpy($1 p, $1 & a1, SA1);
    p += SA1;

    i2->o = nop;
    memcpy($1 p, $1 i2, 1);

    p++;

    i3->o = hlt;
    memcpy($1 p, $1 i3, 1);

    vm->b = S1 + SA1 + S2 + S3;
    vm $ip = (Reg)(vm->m);
    vm $sp = (Reg)(int16)-1;

    // vm $sp = (REGISTERS)((int16)-1);

    free(i1);
    free(i2);

    return (program *)&vm->m;
}

void Printhex(int8 *str, int16 size, int8 delim)
{
    int8 *p;
    int16 n;

    for (p = str, n = size; n; n--, p++)
    {
        printf("%.02x", *p);
        //  %.02x → Prints the byte in two-digit hexadecimal format (e.g., 0A, FF).
        // .02 ensures that even a single-digit hex value (0x5) is printed as 05.

        if (delim)
            printf("%c", delim);
        fflush(stdout);
    }
    printf("\n");

    return;
}

#endif

int main(int, char **)
{
    program *prog;
    VM *vm;

    vm = VirtualMachine();
    printf("VM = %p  (sz : %d)\n", vm, sizeof(VM));

    prog = exampleprog(vm);
    printf("prog = %p\n", prog);

    execute(vm);
    // printf("ax = %04hx\n", $i vm $ax);

    Printhex($1 prog, (map(mov) + map(nop) + map(hlt)), ' ');
}
