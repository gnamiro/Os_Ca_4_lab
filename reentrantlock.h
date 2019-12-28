// Mutual exclusion reentrant lock.
struct reentrantlock {
    uint locked;       // Is the lock held?

    // For debugging:
    char *name;        // Name of lock.
    struct cpu *cpu;   // The cpu holding the lock.
    uint pcs[10];      // The call stack (an array of program counters)
                     // that locked the lock.
    uint rec_counter;   //number of recursion call of process
    uint pid;
};