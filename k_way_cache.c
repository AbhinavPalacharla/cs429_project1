#include "k_way_cache.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static enum CacheReturn mem_access(KWayCache *self, unsigned int address, enum AccessType access_type) {
    unsigned int tag = address >> (self->num_offset_bits + self->num_index_bits); //tag is bits leftover in address without offset and index bits so right shift them off

    unsigned int index_mask = (1 << self->num_index_bits) - 1; //mask to get the num_index_bits bits
    unsigned int index = address >> self->num_offset_bits; //get rid of offset bits
    index &= index_mask; //isolate index bits

    self->access_counter++;

    unsigned int start_index = self->ways * index;

    if(self->mode == WRITE_BACK) {
        //CHECK IF LINE IN CACHE
        for(int i = 0; i < self->ways; i++) {
            if((self->lines[start_index + i].valid == 1) && (self->lines[start_index + i].tag == tag)) {
                self->access_history[start_index + i] = self->access_counter;
                
                if(access_type == DATA_WRITE) {
                    self->lines[start_index + i].dirty = 1;
                }

                return HIT;
            }
        }

        //LINE NOT IN CACHE

        //find victim index
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

        //REPLACE VICTIM
        self->lines[min_index].valid = 1;
        self->lines[min_index].tag = tag;
        self->access_history[min_index] = self->access_counter;

        if(access_type == DATA_WRITE) {
            //if old line dirty then we need to write it back to memory (cache miss)
            if(self->lines[min_index].dirty == 1) {
                self->lines[min_index].dirty = 1; //because data write new line needs to be dirty=1
                return MISS;
            }

            self->lines[min_index].dirty = 1;
            return HIT; //old line not dirty so doesn't need to be written back
        } else {
            //read operation so need to fetch block since not already in cache
            self->lines[min_index].dirty = 0;
            return MISS;
        }
    }

    if(self->mode == WRITE_THROUGH) {

        //CHECK IF LINE IN CACHE
        for(int i = 0; i < self->ways; i++) {
            if((self->lines[start_index + i].valid == 1) && (self->lines[start_index + i].tag == tag)) {
                self->access_history[start_index + i] = self->access_counter;
                
                if(access_type == DATA_WRITE) {
                    return MISS;
                } else {
                    return HIT;
                }
            }
        }

        //LINE NOT IN CACHE

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

        //REPLACE VICTIM
        self->lines[min_index].valid = 1;
        self->lines[min_index].tag = tag;
        self->access_history[min_index] = self->access_counter;

        return MISS;
    }
    
}

KWayCache *init_k_way_cache(int total_size, int block_size, int ways, enum Mode mode) {
    KWayCache *kwc = malloc(sizeof(KWayCache));

    kwc->mode = mode;
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