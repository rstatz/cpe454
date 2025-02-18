#ifndef MMU_H
#define MMU_H

#define PG_SIZE 4096

#define KERNEL_HEAP 0x78000000000
#define KERNEL_STACK 0x8000000000

#define VA_FREE_STACK_SIZE 100

/************************************************************\
Virtual Memory Map:
**************************************************************
Base Address         | Usage
    0x000000000000   |     Physical Page Frames (Identity Map)
    (PML4E SLot 0)   |
    0x010000000000   |     Kernel Stacks
    (PML4E Slot 1)   |     
    0x020000000000   |     Reserved / Growth
    (PML4E Slot 2-14)|     
    0x0F0000000000   |     Kernel Heap
    (PML4E Slot 15)  |
    0x100000000000   |     User Space Starts
    (PML4E Slot 16)  |
\************************************************************/

typedef struct PML4_ref {
    uint16_t reserved_0: 3;
    uint16_t pwt: 1;
    uint16_t pcd: 1;
    uint16_t reserved_1: 7;
    uint64_t base_addr: 40;
    uint16_t reserved_2: 12;
} __attribute__((packed)) PML4_ref;

typedef struct PML4E {
    uint8_t p: 1;
    uint8_t rw: 1;
    uint8_t us: 1;
    uint8_t pwt: 1;
    uint8_t pcd: 1;
    uint8_t a: 1;
    uint8_t ign: 1;
    uint16_t mbz: 2;
    uint16_t avl_lo: 3;
    uint64_t base_addr: 40;
    uint16_t avl_hi: 11;
    uint16_t nx: 1;
} __attribute__((packed)) PML4E;

typedef struct L3E_G {
    uint8_t p: 1;
    uint8_t rw: 1;
    uint8_t us: 1;
    uint8_t pwt: 1;
    uint8_t pcd: 1;
    uint8_t a: 1;
    uint8_t d: 1;
    uint8_t s: 1;
    uint8_t g: 1;
    uint8_t avl: 3;
    uint8_t pat: 1;
    uint32_t res: 17;
    uint32_t base_addr: 22;
    uint16_t avl_hi: 11;
    uint8_t nx: 1;
} __attribute__((packed)) L3E_G;

typedef struct L3E {
    uint8_t p: 1;
    uint8_t rw: 1;
    uint8_t us: 1;
    uint8_t pwt: 1;
    uint8_t pcd: 1;
    uint8_t a: 1;
    uint8_t ign: 1;
    uint8_t s: 1; 
    uint8_t mbz: 1;
    uint8_t avl_lo: 3;
    uint64_t base_addr: 40;
    uint16_t avl_hi: 11;
    uint8_t nx: 1;
} __attribute__((packed)) L3E;

typedef struct L2E {
    uint8_t p: 1;
    uint8_t rw: 1;
    uint8_t us: 1;
    uint8_t pwt: 1;
    uint8_t pcd: 1;
    uint8_t a: 1;
    uint8_t ign: 1;
    uint8_t s: 1;
    uint8_t ign2: 1;
    uint8_t avl_lo: 3;
    uint64_t base_addr: 40;
    uint16_t avl_hi: 11;
    uint8_t nx: 1;
} __attribute__((packed)) L2E;

typedef struct L1E {
    uint8_t p: 1;
    uint8_t rw: 1;
    uint8_t us: 1;
    uint8_t pwt: 1;
    uint8_t pcd: 1;
    uint8_t a: 1;
    uint8_t d: 1;
    uint8_t pat: 1;
    uint8_t g: 1;
    uint8_t avl_lo: 3;
    uint64_t base_addr: 40;
    uint16_t avl_hi: 11;
    uint8_t nx: 1;
} __attribute__((packed)) L1E;

typedef struct vaddr {
    uint16_t phys_offset: 12;
    uint16_t pt_offset: 9;
    uint16_t pd_offset: 9;
    uint16_t pdp_offset: 9;
    uint16_t pml4_offset: 9;
    uint16_t se: 16;
} __attribute__((packed)) vaddr;

typedef struct virt_map {
    PML4E* pml4_base;
    uint64_t lost_mem_stat;
    uint64_t *vakhsp, va_kheap_last;
    uint64_t *vakssp, va_kstack_last;
    uint64_t va_kheap_free_stack[VA_FREE_STACK_SIZE];
    uint64_t va_kstack_free_stack[VA_FREE_STACK_SIZE];
} virt_map;

static inline vaddr get_pf_va() {
    uint64_t val;

    asm volatile("mov %%cr2, %0" : "=a"(val) : );
    
    return *(vaddr*)&val;
}

static inline PML4_ref get_pt_addr() {
    PML4_ref addr;

    asm volatile("mov %%cr3, %0" : "=a"(addr) : );
    
    return addr;
}

static inline void set_pt(PML4_ref pml) {
    asm volatile("mov %0, %%cr3" : : "Nd"(pml));
}

void MMU_init(void* tag);

void* MMU_pf_alloc();
void MMU_pf_free(void*);

void* MMU_alloc_page();
void* MMU_alloc_kstack();

void MMU_free_page(void*);
void MMU_free_kstack(void*);

void IRQ_pf_handler(int);

#endif
