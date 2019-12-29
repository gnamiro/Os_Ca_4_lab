#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "barrier.h"
#include "reentrantlock.h"
#include "spinlock.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
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

int
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

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_barrier_init(void)
{
  int variable;
  if(argint(0, &variable) < 0){
    cprintf("Kernel: System call returning -1\n");
    return -1;
  }
  return barrier_init(variable);
}

int sys_barrier_check(void)
{
  return barrier_check();
}

int sys_reentrant_lock(void){
  struct reentrantlock lock;
  struct spinlock _lock;

  initrelock(&lock,"reentrantLock");
  initlock(&_lock,"spinlock");

  relockacquire(&lock);
  cprintf("acquire reentrant lock\n");
  relockacquire(&lock);
  cprintf("acquire reentrant lock again\n");
  relockacquire(&lock);
  cprintf("acquiret reentrant lock again again\n");

  relockrelease(&lock);
  cprintf("release reentrant lock\n");

  acquire(&_lock);
  cprintf("acquire spinlock \n");
  acquire(&_lock);
  cprintf("it cannot be happened\n");
  
  release(&_lock);
  cprintf("okay every thing is absolutley wrong\n");
  return 1;
}