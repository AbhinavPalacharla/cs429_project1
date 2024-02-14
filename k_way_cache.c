#include "k_way_cache.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ADDRESS_SIZE 32

static int mem_access(KWayCache *self, unsigned int address, int write_miss) {
    int num_offset_bits = log2(self->block_size);
    int num_index_bits = log2(self->num_lines / self->ways);

    // unsigned int tag = address >> (num_offset_bits + num_index_bits);
    // unsigned int index = (int) ((address >> num_offset_bits) & num_index_bits);

    unsigned int tag = address >> (num_offset_bits + num_index_bits);
    unsigned int mask = (1 << num_index_bits) - 1;
    unsigned int index = (int) ((address >> num_offset_bits) & mask);

    self->access_counter += 1;

    if(!write_miss) {
        for(int i = 0; i < (self->ways - 1); i++) {
            if((self->lines[(self->ways * index) + i].valid == 1) && (self->lines[(self->ways * index) + i].tag == tag)) {
                //CACHE HIT
                printf("(HIT) ADDR: %X INDEX: %X I: %d TAG: %X\n", address, index, i, tag);
                self->access_history[i] = self->access_counter;
                return 1;
            }
        }
    }

    //CACHE MISS

    //find victim index or check if valid bit is zero and insert there
    int minIndex = (self->ways * index);

    for(int i = 0; i < (self->ways - 1); i++) {
        if(self->lines[(self->ways * index) + i].valid == 0) {
            minIndex = i;
            break;
        }
        if((self->access_history[(self->ways * index) + i]) < (self->access_history[(self->ways * index) + minIndex])) {
            minIndex = i;
        }
    }

    printf("(MISS) PREV_TAG: %X ADDR: %X INDEX: %X I: %d TAG: %X\n", self->lines[(self->ways * index) + minIndex].tag, address, index, minIndex, tag);

    //replace victim
    self->lines[(self->ways * index) + minIndex].valid = 1;
    self->lines[(self->ways * index) + minIndex].tag = tag;

    return 0;
}

KWayCache *init_k_way_cache(int total_size, int block_size, int ways) {
    KWayCache *kwc = malloc(sizeof(KWayCache));

    kwc->ways = ways;
    kwc->num_lines = total_size / block_size;
    kwc->block_size = block_size;
    kwc->access_counter = 0;
    kwc->access_history = malloc(sizeof(int) * kwc->num_lines);

    for(int i = 0; i < kwc->num_lines; i++) {
        kwc->access_history[i] = 0;
    }

    kwc->lines = malloc(sizeof(KWayCacheLine) * kwc->num_lines);

    for(int i = 0; i < kwc->num_lines; i++) { kwc->lines[i].valid = 0; }

    kwc->mem_access = &mem_access;

    return kwc;
}