/*----------xv6 sync lab----------*/
#include "types.h"
#include "x86.h"
#include "spinlock.h"
#include "defs.h"
#include "barrier.h"

//define any variables needed here

struct spinlock barrierlock;

int num; // size of barrier


void barrierlockinit(void) {

    initlock(&barrierlock, "barrierlock");
    // initlock(&barrierlock2, "barrierlock2");

}

int
barrier_init(int n)
{
    num = n;
    return 0;
}

int
barrier_check(void)
{

    acquire(&barrierlock);

    num--;

    if (num == 0) {
        wakeup(&num);

    }
    else if (num > 0) {
        sleep(&num, &barrierlock);
    }


    release(&barrierlock);


    return 0;
}

/*----------xv6 sync lock end----------*/
