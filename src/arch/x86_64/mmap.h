#include "stdint.h"

#ifndef MMAP_H
#define MMAP_H

typedef struct MB_tag {
    uint32_t tag_type;
    uint32_t tag_size;
} __attribute__((packed)) MB_tag;

typedef struct MB_tag_header {
    uint32_t tag_size;
    uint32_t reserved_0;
} __attribute__((packed)) MB_tag_header;

typedef struct MB_tag_basic_mem {
    uint32_t tag_type;
    uint32_t tag_size;
    uint32_t low_mem_size;
    uint32_t high_mem_size;
} __attribute__((packed)) MB_tag_basic_mem;

typedef struct MB_tag_BIOS_boot_dev {
    uint32_t tag_type;
    uint32_t tag_size;
    uint32_t boot_dev;
    uint32_t boot_part;
    uint32_t boot_subpart;
} __attribute__((packed)) MB_tag_BIOS_boot_dev;

typedef struct MB_tag_boot_cmd {
    uint32_t tag_type;
    uint32_t tag_size;
    uint32_t cmd_line;
} __attribute__((packed)) MB_tag_boot_cmd;

typedef struct MB_tag_bootloader {
    uint32_t tag_type;
    uint32_t tag_size;
    uint32_t bootloader_name;
} __attribute__((packed)) MB_tag_bootloader;

typedef struct mem_region {
    uint64_t base_addr;
    uint64_t length;
} __attribute__((packed)) mem_region;

typedef struct MB_mem_info {
    mem_region region;
    uint32_t type;
    uint32_t reserved;
} __attribute__((packed)) MB_mem_info;

typedef struct MB_tag_mmap {
    uint32_t tag_type;
    uint32_t tag_size;
    uint32_t mem_info_size;
    uint32_t mem_info_vers;
    MB_mem_info mem_info_entries[0];
} __attribute__((packed)) MB_tag_mmap;

typedef struct mem_map {
    uint32_t num_avl, num_unavl, curr_region;
    mem_region mem_avl[20];
    mem_region mem_unavl[20];
    void *pg_last, *pgs_free;
    uint64_t pgs_alloc, pgs_avl;
} mem_map;

typedef struct MB_tag_elf_symbols {
    uint32_t tag_type;
    uint32_t tag_size;
    uint32_t num_sect_head_entries;
    uint32_t sect_head_size;
    uint32_t index_str_table_sect;
    uint32_t sect_head_array;
} __attribute__((packed)) MB_tag_elf_symbols;

typedef struct MB_elf_sect_header {
    uint32_t sect_name_index; // index into string table
    uint32_t sect_type;
    uint64_t flags;
    uint64_t seg_addr;
    uint64_t seg_offset;
    uint64_t seg_size;
    uint32_t table_index_link;
    uint32_t extra_info;
    uint64_t address_align;
    uint64_t entry_size;
} __attribute__((packed)) MB_elf_sect_header;

typedef struct pf_header {
    void* next;
} pf_header;

void mmap_setup(mem_map*, void*);

#endif
