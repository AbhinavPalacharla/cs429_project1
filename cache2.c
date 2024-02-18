#include "cache.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static enum CacheReturn mem_access (DirectCache *self, unsigned int address, enum AccessType access_type) {
    unsigned int tag = address >> (self->num_offset_bits + self->num_index_bits); //tag is bits leftover in address without offset and index bits so right shift them off

    unsigned int index_mask = (1 << self->num_index_bits) - 1; //mask to get the num_index_bits bits
    unsigned int index = address >> self->num_offset_bits; //get rid of offset bits
    index &= index_mask; //isolate index bits

    //CACHE HIT
    if((self->lines[index].valid == 1) && (self->lines[index].tag == tag)) {
        // printf("(HIT) ADDR: %X INDEX: %X TAG: %X\n", address, index, tag);
        if(access_type == DATA_WRITE) { 
            
            if(self->mode == WRITE_BACK) {
            //line already in cache so don't need to access main mem, just set dirty bit
                self->lines[index].dirty = 1;
            } else {
                //write through cache so count it as miss
                return MISS;
            }
        }
        
        return HIT;
    }

    // CACHE MISS
    // printf("(MISS) PREV_TAG: %X ADDR: %X INDEX: %X TAG: %X\n", self->lines[index].tag, address, index, tag);
    
    self->lines[index].valid = 1;
    self->lines[index].tag = tag; //store new tag

    if(self->mode == WRITE_BACK) {
        if(self->lines[index].dirty == 0) {
            if(access_type == DATA_WRITE) {self->lines[index].dirty = 1;} else {self->lines[index].dirty = 0;}
            return HIT;
        }
            
        if(access_type == DATA_WRITE) {self->lines[index].dirty = 1;} else {self->lines[index].dirty = 0;}

    }
    
    return MISS;
}

DirectCache *init_direct_cache(int total_size, int block_size, enum Mode mode) {
    DirectCache *dc = malloc(sizeof(DirectCache));

    dc->mode = mode;
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