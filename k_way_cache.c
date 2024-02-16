#include "k_way_cache.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static int mem_access(KWayCache *self, unsigned int address, int write_miss) {
    unsigned int tag = address >> (self->num_offset_bits + self->num_index_bits); //tag is bits leftover in address without offset and index bits so right shift them off

    unsigned int index_mask = (1 << self->num_index_bits) - 1; //mask to get the num_index_bits bits
    unsigned int index = address >> self->num_offset_bits; //get rid of offset bits
    index &= index_mask; //isolate index bits

    self->access_counter++;

    unsigned int start_index = self->ways * index;

    // if(!write_miss) {
    for(int i = 0; i < self->ways; i++) {
        if((self->lines[start_index + i].valid == 1) && (self->lines[start_index + i].tag == tag)) {
            //CACHE HIT
            // printf("(HIT) ADDR: %X TAG: %X\n", address, tag);
            self->access_history[start_index + i] = self->access_counter;
            
            if(write_miss) { return 0; }
            
            return 1;
        }
    }
    // }

    //CACHE MISS

    //find victim index or check if valid bit is zero and insert there
    int min_index = start_index;

    //loop from start index of set to end index of set
    for(int i = 0; i < self->ways; i++) {
        if(self->lines[start_index + i].valid == 0) {
            //found empty line so this is victim
            min_index = start_index + i;
            break;
        }
        if((self->access_history[start_index + i]) < (self->access_history[min_index])) {
            min_index = start_index + i;
        }
    }

    // printf("(MISS) PREV_TAG: %X ADDR: %X TAG: %X\n", self->lines[min_index].tag, address, tag);

    //replace victim
    self->lines[min_index].valid = 1;
    self->lines[min_index].tag = tag;
    self->access_history[min_index] = self->access_counter;

    return 0;
}

KWayCache *init_k_way_cache(int total_size, int block_size, int ways) {
    KWayCache *kwc = malloc(sizeof(KWayCache));

    kwc->ways = ways;
    kwc->num_lines = total_size / block_size;
    kwc->block_size = block_size;
    kwc->access_counter = 0;
    kwc->access_history = malloc(sizeof(int) * kwc->num_lines);
    kwc->num_offset_bits = log2(block_size);
    kwc->num_index_bits = log2((kwc->num_lines / ways));

    for(int i = 0; i < kwc->num_lines; i++) {
        kwc->access_history[i] = 0;
    }

    kwc->lines = malloc(sizeof(KWayCacheLine) * kwc->num_lines);

    for(int i = 0; i < kwc->num_lines; i++) { kwc->lines[i].valid = 0; }

    kwc->mem_access = &mem_access;

    return kwc;
}