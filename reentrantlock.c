// Mutual exclusion spin locks.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "reentrantlock.h"

void
initrelock(struct reentrantlock *lk, char *name)
{
  lk->name = name;
  lk->locked = 0;
  lk->cpu = 0;
  lk->rec_counter =0;
}

// Acquire the lock.
// Loops (spins) until the lock is acquired.
// Holding a lock for a long time may cause
// other CPUs to waste time spinning to acquire it.
void
relockacquire(struct reentrantlock *lk)
{
  pushcli(); // disable interrupts to avoid deadlock.

  
  // The xchg is atomic.            lk->cpu != mycpu() --> Check whether this cpu is holding the same lock.
  while(xchg(&lk->locked, 1) != 0 && lk->cpu != mycpu())
    ;

  // Tell the C compiler and the processor to not move loads or stores
  // past this point, to ensure that the critical section's memory
  // references happen after the lock is acquired.
  __sync_synchronize();

  // Record info about lock acquisition for debugging.
  struct cpu *cpu = mycpu();
  lk->cpu = cpu;
  lk->rec_counter++;
  getcallerpcs(&lk, lk->pcs);
}

// Release the lock.
void
relockrelease(struct reentrantlock *lk)
{
    if(lk->rec_counter == 1){

        lk->pcs[0] = 0;
        lk->cpu = 0;
        lk->rec_counter = 0;

        // Tell the C compiler and the processor to not move loads or stores
        // past this point, to ensure that all the stores in the critical
        // section are visible to other cores before the lock is released.
        // Both the C compiler and the hardware may re-order loads and
        // stores; __sync_synchronize() tells them both not to.
        __sync_synchronize();

        // Release the lock, equivalent to lk->locked = 0.
        // This code can't use a C assignment, since it might
        // not be atomic. A real OS would use C atomics here.
        asm volatile("movl $0, %0" : "+m" (lk->locked) : );

        popcli();

    }else{

        lk->rec_counter -= 1;
    }
}
