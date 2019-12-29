
#include "types.h"
#include "x86.h"
#include "spinlock.h"
#include "defs.h"
#include "barrier.h"

//define any variables needed here



struct barrier{
    struct spinlock lock;
    int num; // size of barrier
};

struct barrier barriers[100];

void barrierlockinit(int i) {

    initlock(&barriers[i].lock, "barrierlock");
    // initlock(&barrierlock2, "barrierlock2");

}

int
barrier_init(int n, int i)
{
    barriers[i].num = n;
    return 0;
}

int
barrier_check(int i)
{

    acquire(&barriers[i].lock);

    barriers[i].num--;

    if (barriers[i].num == 0) {
        wakeup(&barriers[i].num);

    }
    else if (barriers[i].num > 0) {
        sleep(&barriers[i].num, &barriers[i].lock);
    }


    release(&barriers[i].lock);


    return 0;
}
