#ifndef PID_MANAGER_H
#define PID_MANAGER_H

#define PAGE_SIZE (sizeof(unsigned int))
#define BITS_PER_PAGE (PAGE_SIZE * 8)
#define BITS_PER_PAGE_MASK (BITS_PER_PAGE-1)

#define MIN_PID 300
#define MAX_PID 5000

#define MAX_PAGE ((MAX_PID + BITS_PER_PAGE - 1) / BITS_PER_PAGE)

struct pidmap{
    unsigned int num_free;
    unsigned int page;
};

struct pids{
    struct pidmap map[MAX_PAGE];
    int last_pid;
};

int allocate_map(void);
int allocate_pid(void);
void release_pid(int pid);

#endif