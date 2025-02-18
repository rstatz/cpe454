#include "ist.h"
#include "print.h"
#include "debug.h"

#define MASK_4 0xF
#define MASK_8 0xFF
#define MASK_16 0xFFFF
#define MASK_32 0xFFFFFFFF

TSS tss;

extern void* stack_ist1_top;
extern void* stack_ist2_top;
extern void* stack_ist3_top;
extern void* stack_ist4_top;
extern void* stack_ist5_top;
extern void* stack_ist6_top;
extern void* stack_ist7_top;

extern void* stack_rsp0_top;
extern void* stack_rsp1_top;
extern void* stack_rsp2_top;

TSSdesc new_TSSdesc() {
    TSSdesc t;

    t.zero = 0;
    
    t.reserved_1 = 0;
    t.reserved_2 = 0;

    t.g = 0;
    t.dpl = 0;
    t.avl = 0;
    t.type = 0b1001;
    t.present = 1;

    t.seg_limit_low = (sizeof(tss) - 1) & MASK_16;
    t.seg_limit_up = (sizeof(tss) >> 16) & MASK_4;

    t.base_addr_up = (((uint64_t)(&tss)) >> 32) & MASK_32;
    t.base_addr_mup = (((uint64_t)(&tss)) >> 24) & MASK_8;
    t.base_addr_mlow = (((uint64_t)(&tss)) >> 16) & MASK_8;
    t.base_addr_low = ((uint64_t)(&tss)) & MASK_16;

    return t;
}

void load_task_register(uint64_t tss_byte_offset) {
    asm volatile("ltr %w0" : : "r" (tss_byte_offset));
}

void init_TSS() {
    tss.reserved_io = 0;
    tss.reserved_0 = 0;
    tss.reserved_1 = 0;
    tss.reserved_2 = 0;

    tss.io_base = sizeof(tss);

    tss.rsp0 = (uint64_t)&stack_rsp0_top;
    tss.rsp1 = (uint64_t)&stack_rsp1_top;
    tss.rsp2 = (uint64_t)&stack_rsp2_top;

    tss.ist1 = (uint64_t)&stack_ist1_top;
    tss.ist2 = (uint64_t)&stack_ist2_top;
    tss.ist3 = (uint64_t)&stack_ist3_top;
    tss.ist4 = (uint64_t)&stack_ist4_top;
    tss.ist5 = (uint64_t)&stack_ist5_top;
    tss.ist6 = (uint64_t)&stack_ist6_top;
    tss.ist7 = (uint64_t)&stack_ist7_top;
}
