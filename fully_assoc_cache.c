#include "fully_assoc_cache.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static int mem_access(FullyAssocCache *self, unsigned int address, int write_miss) {
    unsigned int tag = address >> self->num_offset_bits;

    self->access_counter++;

    for(int i = 0; i < self->num_lines; i++) {
        if((self->lines[i].valid == 1) && (self->lines[i].tag == tag)) {
            self->access_history[i] = self->access_counter;
            
            if(write_miss) {return 0;}
            
            return 1;
        }
    }

    int min_index = 0;
    int min_value = self->access_history[0];

    for(int i = 0; i < (self->num_lines); i++) {
        if(self->access_history[i] < min_value) {
            min_index = i; min_value = self->access_history[i];
        }
    }

    //replace victim
    self->lines[min_index].valid = 1;
    self->lines[min_index].tag = tag;
    self->access_history[min_index] = self->access_counter;

    return 0;
}

FullyAssocCache *init_fully_assoc_cache(int total_size, int block_size) {
    FullyAssocCache *fac = malloc(sizeof(FullyAssocCache));

    fac->num_lines = total_size / block_size;
    fac->block_size = block_size;
    fac->total_size = total_size;
    fac->num_offset_bits = log2(block_size);
    fac->access_counter = 0;
    fac->access_history = malloc(sizeof(int) * fac->num_lines);

    for(int i = 0; i < fac->num_lines; i++) {
        fac->access_history[i] = 0;
    }

    fac->lines = malloc(sizeof(FullyAssocCacheLine) * fac->num_lines);

    for(int i = 0; i < fac->num_lines; i++) { fac->lines[i].valid = 0; }

    fac->mem_access = &mem_access;

    return fac;
}