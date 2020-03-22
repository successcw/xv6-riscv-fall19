#include "kernel/syscall.h"
#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    char *temp;
    printf("==========cw_sbrk test\n");
    getpt();
    temp = sbrk(1);
    printf("==========cw_sbrk test %x\n", temp);
    getpt();
    exit();
}
