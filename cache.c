#include "cache.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static int mem_access(DirectCache *self, unsigned int address, int write_miss) {
    unsigned int tag = address >> (self->num_offset_bits + self->num_index_bits); //tag is bits leftover in address without offset and index bits so right shift them off

    unsigned int index_mask = (1 << self->num_index_bits) - 1; //mask to get the num_index_bits bits
    unsigned int index = address >> self->num_offset_bits; //get rid of offset bits
    index &= index_mask; //isolate index bits

    if((self->lines[index].valid == 1) && (self->lines[index].tag == tag)) {
        //CACHE HIT
        // printf("(HIT) ADDR: %X INDEX: %X TAG: %X\n", address, index, tag);
        if(write_miss) { return 0; }
        
        return 1;
    }

    // CACHE MISS
    // printf("(MISS) PREV_TAG: %X ADDR: %X INDEX: %X TAG: %X\n", self->lines[index].tag, address, index, tag);
    
    self->lines[index].valid = 1;
    self->lines[index].tag = tag;

    return 0;
}

DirectCache *init_direct_cache(int total_size, int block_size) {
    DirectCache *dc = malloc(sizeof(DirectCache));

    dc->total_size = total_size;
    dc->block_size = block_size;
    dc->num_lines = total_size / block_size;
    dc->num_offset_bits = log2(block_size);
    dc->num_index_bits = log2((dc->num_lines));
    
    dc->lines = malloc(sizeof(DirectCacheLine) * dc->num_lines);

    for(int i = 0; i < dc->num_lines; i++) { dc->lines[i].valid = 0; }
    
    dc->mem_access = &mem_access;

    return dc;
}