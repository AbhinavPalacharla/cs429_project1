#include "cache.h"

#include <stdio.h>
#include <stdlib.h>

static int mem_access(DirectCache *self, unsigned int address, int write_miss) {
    unsigned int tag = address >> 10;

    // unsigned int sec_last_5_mask = 0x3E0;
    unsigned int index = (address >> 5) & 0x1F;
    // index >>= 5;

    if(!write_miss && (self->lines[index].valid == 1) && (self->lines[index].tag == tag)) {
        //CACHE HIT
        printf("(HIT) ADDR: %X INDEX: %X TAG: %X\n", address, index, tag);
        return 1;
    }

    //CACHE MISS
    printf("(MISS) PREV_TAG: %X ADDR: %X INDEX: %X TAG: %X\n", self->lines[index].tag, address, index, tag);
    
    self->lines[index].valid = 1;
    self->lines[index].tag = tag;

    return 0;
}

DirectCache *init_direct_cache(int total_size, int block_size) {
    DirectCache *dc = malloc(sizeof(DirectCache));

    int num_cache_lines = total_size / block_size;
    
    dc->lines = malloc(sizeof(DirectCacheLine) * num_cache_lines);

    for(int i = 0; i < num_cache_lines; i++) { dc->lines[i].valid = 0; }
    
    dc->mem_access = &mem_access;

    return dc;
}