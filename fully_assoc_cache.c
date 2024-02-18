#include "fully_assoc_cache.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static enum CacheReturn mem_access(FullyAssocCache *self, unsigned int address, enum AccessType access_type) {
    unsigned int tag = address >> self->num_offset_bits;

    self->access_counter++;

    if(self->mode == WRITE_BACK) {

        /* <<< LINE IN CACHE >>> */
        for(int i = 0; i < self->num_lines; i++) {
            if((self->lines[i].valid == 1) && (self->lines[i].tag == tag)) {
                self->access_history[i] = self->access_counter;

                if(access_type == DATA_WRITE) {
                    self->lines[i].dirty = 1; //set dirty bit and return HIT cuz already in cache
                }
                
                return HIT;
            }
        }

        /* <<< NOT IN CACHE >>> */

        //FIND LINE TO REPLACE
        int min_index = 0;
        int min_value = self->access_history[0];

        for(int i = 0; i < (self->num_lines); i++) {
            if(self->access_history[i] < min_value) {
                min_index = i; min_value = self->access_history[i];
            }
        }

        //REPLACE VICTIM
        self->lines[min_index].valid = 1;
        self->lines[min_index].tag = tag;
        self->access_history[min_index] = self->access_counter;

        if(access_type == DATA_WRITE) {
            //if old line dirty then write it back (MISS)
            if(self->lines[min_index].dirty == 1) {
                self->lines[min_index].dirty = 1;
                return MISS;
            }

            //old line not dirty so dont need to write back (HIT)
            self->lines[min_index].dirty = 1;
            return HIT;
        } else {
            //read operation so cache MISS because need to fetch block
            //and reset dirty bit to zero
            self->lines[min_index].dirty = 0;
            return MISS;
        }
    }

    if(self->mode == WRITE_THROUGH) {

        for(int i = 0; i < self->num_lines; i++) {
            if((self->lines[i].valid == 1) && (self->lines[i].tag == tag)) {
                self->access_history[i] = self->access_counter;

                if(access_type == DATA_WRITE) {
                    return MISS;
                }
                
                return HIT;
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

        return MISS;

    }
}

FullyAssocCache *init_fully_assoc_cache(int total_size, int block_size, enum Mode mode) {
    FullyAssocCache *fac = malloc(sizeof(FullyAssocCache));

    fac->mode = mode;
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