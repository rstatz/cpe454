#include <stdint.h>
#include <stddef.h>
#include "mmap.h"
#include "mmu.h"
#include "print.h"

#define PG_SIZE 4096
#define DFLT_NUM_PGS 100

mem_map mmap;

int add_pf() {
    mem_region r;
    uint64_t end;

    // Computes end of available region
    r = mmap.mem_avl[mmap.curr_region];
    end = r.base_addr + r.length - 1;
    end = ((end % PG_SIZE) == 0) ? end : (end + (end % PG_SIZE));

    // Automatically ignores page starting at zero
    mmap.pg_last += PG_SIZE;

    // check if room for a page in current region
    while ((mmap.pg_last >= (void*)end) 
            || ((mmap.pg_last + PG_SIZE) > (void*)end)) {
        mmap.curr_region++;

        if (mmap.curr_region >= mmap.num_avl)
            return 0;

        r = mmap.mem_avl[mmap.curr_region];
        end = r.base_addr + r.length - 1;
        end = ((end % PG_SIZE) == 0) ? end : (end + (end % PG_SIZE));
        mmap.pg_last = (void*)(r.base_addr + PG_SIZE - (r.base_addr % PG_SIZE));
        // Make sure logic is correct
    }

    if (mmap.curr_region >= mmap.num_avl)
        return 0; 

    ((pf_header*)mmap.pg_last)->next = mmap.pgs_free;
    mmap.pgs_free = mmap.pg_last;
    mmap.pgs_avl++;

    return 1;
}

void init_mem_pool() {
    mmap.pg_last = (void*)mmap.mem_avl[0].base_addr;

    while (mmap.pgs_avl != DFLT_NUM_PGS) {
        if(!add_pf())
            break;
    }
}

void init_idpaging(PML4_ref pml) {
    PML4E* pml4_base;

    pml4_base = (PML4E*)(uint64_t)pml.base_addr;

    pml4_base[0].p = 1;
    pml4_base[0].pat = 1; // Set huge bit, fill direct map the entire thing
}

void MMU_init(PML4_ref pml, void* tag) {
    mmap_setup(&mmap, tag);
    init_mem_pool();
    init_idpaging(pml);
}

void* MMU_pf_alloc() {
    void* pf;

    // return NULL if no pages left to allocate
    if (!mmap.pgs_avl && !add_pf()) {
        printk("OUT OF MEMORY\n");
        return NULL;
    }

    pf = mmap.pgs_free;
    mmap.pgs_free = ((pf_header*)mmap.pgs_free)->next;
    mmap.pgs_avl--;
    mmap.pgs_alloc++;

    return pf;
}

void MMU_pf_free(void* pf) {
    ((pf_header*)pf)->next = mmap.pgs_free;
    mmap.pgs_free = pf;
    mmap.pgs_alloc--;
    mmap.pgs_avl++;
}
