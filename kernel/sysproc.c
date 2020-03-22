#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  return wait();
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

void walk_pt(pagetable_t pagetable, int level)
{
  pagetable_t pagetable_temp;
  for (int i = 0; i < 512; i++) {
      pte_t *pte = &pagetable[i];
      if (*pte & PTE_V) {
          printf("PTE level:%d index:%d\n", level, i);
          if (level == 0){
              printf("PTE addr:%p\n", PTE2PA(*pte));
              continue;
          } else {
              pagetable_temp = (pagetable_t)PTE2PA(*pte);
              walk_pt(pagetable_temp, level-1);
          }
      }
  }
}

uint64
sys_getpt(void)
{
  struct proc *p = myproc();
  printf("PID: %d name: %s pt:%p\n",
          p->pid, p->name, p->pagetable);
  printf("satp:%p\n", r_satp());
  printf("kernel pagetable:%p\n", p->tf->kernel_satp);
  walk_pt(p->pagetable, 2);
  return 0;
}
