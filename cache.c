#include "cache.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static int mem_access(DirectCache *self, unsigned int address, int write_miss) {
    // unsigned int tag = address >> 10;

    // // unsigned int sec_last_5_mask = 0x3E0;
    // unsigned int index = (address >> 5) & 0x1F;
    // // index >>= 5;

    // if(!write_miss && (self->lines[index].valid == 1) && (self->lines[index].tag == tag)) {
    //     //CACHE HIT
    //     printf("(HIT) ADDR: %X INDEX: %X TAG: %X\n", address, index, tag);
    //     return 1;
    // }

    // //CACHE MISS
    // printf("(MISS) PREV_TAG: %X ADDR: %X INDEX: %X TAG: %X\n", self->lines[index].tag, address, index, tag);
    
    // self->lines[index].valid = 1;
    // self->lines[index].tag = tag;



    // return 0;

    int num_offset_bits = log2(self->block_size);
    int num_index_bits = log2((self->num_lines));

    unsigned int tag = address >> (num_offset_bits + num_index_bits); //tag is bits leftover in address without offset and index bits so right shift them off

    unsigned int offset_mask = (1 << num_offset_bits) - 1; //mask to get last num_offset_bits bits
    unsigned int offset = address & offset_mask;

    unsigned int index_mask = (1 << num_index_bits) - 1; //mask to get the num_index_bits bits
    unsigned int index = address >> num_offset_bits; //get rid of offset bits
    index &= index_mask; //isolate index bits

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

    dc->total_size = total_size;
    dc->block_size = block_size;
    // int num_cache_lines = total_size / block_size;
    dc->num_lines = total_size / block_size;
    
    dc->lines = malloc(sizeof(DirectCacheLine) * dc->num_lines);

    for(int i = 0; i < dc->num_lines; i++) { dc->lines[i].valid = 0; }
    
    dc->mem_access = &mem_access;

    return dc;
}