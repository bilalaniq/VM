#include "../include/utils.h"

VM *VirtualMachine(program *pr, int16 progSZ)
{
    VM *p;
    int16 size;
    program *pp;

    assert((pr) && (progSZ));

    size = $2 sizeof(VM);
    p = (VM *)malloc($i size);

    if (!p)
    {
        // errno = ErrMem;

        return (VM *)0;
    }

    memset(p, 0, size);

    pp = (program *)malloc($i progSZ);

    if (!pp)
    {
        free(p);
        // errno = ErrMem;
        return (VM *)0;
    }

    memcpy(pp, pr, progSZ);

    return p;
}

int main(int, char **)
{
    printf("Hello, World!\n");
    return 0;
}
