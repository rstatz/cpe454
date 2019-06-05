#include "test.h"

#include <stddef.h>

#include "mmap.h"
#include "mmu.h"
#include "strings.h"
#include "debug.h"
#include "print.h"

#define PG_SIZE 4096

void fill_pf(void* pf) {
    uint32_t i;

    for (i = 0; i < (PG_SIZE/sizeof(void*)); i++)
        ((void**)pf)[i] = pf;
}

void test_pf_alloc() {
    int i;
    void* pgs[10];

    for (i = 0; i < 10; i++) {
        pgs[i] = MMU_pf_alloc();
        printk("Alloc'd pf at %p\n", pgs[i]);
    //    fill_pf(pgs[i]);
    }

    for (i = 0; i < 5; i++) {
        MMU_pf_free(pgs[i]);
        printk("Freed pf at %p\n", pgs[i]); 
   }

    pgs[0] = MMU_pf_alloc();
    printk("Alloc'd pf at %p\n", pgs[0]);
    //fill_pf(pgs[0]);
    
    for (i = 5; i < 10; i++) {
        MMU_pf_free(pgs[i]);
        printk("Freed pf at %p\n", pgs[i]);
    }

    MMU_pf_free(pgs[0]);
    printk("Freed pf at %p\n", pgs[0]);

    MMU_pf_alloc(); 
//    BREAK;
    while ((pgs[0] = MMU_pf_alloc()) != NULL)
        printk("%p\n", pgs[0]);
}
void test_alloc_page() {
    int i;
    void* vaddr[10];

//    BREAK;    

    for (i = 0; i < 10; i++) {
        vaddr[i] = MMU_alloc_page();
//        printk("Alloc'd va %p\n", vaddr[i]);
    }

    MMU_free_page(vaddr[0]);
//    printk("Freed va %p\n", vaddr[0]);
    vaddr[0] = MMU_alloc_page();
//    printk("Realloc'd va %p\n", vaddr[0]);

//    BREAK;
    memset(vaddr[1], 0, 8);

    for (i = 0; i < 10; i++) {
        MMU_free_page(vaddr[i]);
//        printk("Freed va %p\n", vaddr[i]);
    }
}
