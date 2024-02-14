#include "operation.h"
#include "cache.h"
#include "k_way_cache.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOTAL_SIZE 1024
#define BLOCK_SIZE 32
#define SET_ASSOC 2 //1(direct mapped), 2-30(k-way assoc), 32(fully assoc)


int main(int argc, char **argv) {
    if(argc != 2) {
        printf("ERROR: expected argument trace filename\nFORMAT: ./a.out <trace_file.trace>");
        exit(1);
    }

    FILE *f = fopen(argv[1], "r");

    if(f == NULL) {
        perror("Error opening file");
    }

    //LOAD ALL OPERATIONS FROM FILE INTO OPERATIONS QUEUE

    // DirectCache *i_dc = init_direct_cache(TOTAL_SIZE, BLOCK_SIZE);
    // DirectCache *d_dc = init_direct_cache(TOTAL_SIZE, BLOCK_SIZE);
    KWayCache *i_kwc = init_k_way_cache(TOTAL_SIZE, BLOCK_SIZE, SET_ASSOC);
    KWayCache *d_kwc = init_k_way_cache(TOTAL_SIZE, BLOCK_SIZE, SET_ASSOC);

    int i_hits = 0; int i_misses = 0;
    int d_hits = 0; int d_misses = 0;

    size_t len = 128; char *line = malloc(sizeof(char) * len);  ssize_t read;

    while((read = getline(&line, &len, f)) != -1) {
        enum AccessType access_type;
        access_type = atoi(&(line[0]));

        memmove(line, line + 2, strlen(line) - 1);
        line[strlen(line) - 1] = '\0';
        char *hex = line;

        char *endptr;

        Operation *op = init_op(access_type, strtol(hex, &endptr, 16));

        // if(access_type == INSTRUCTION_READ) {
        //     if(i_dc->mem_access(i_dc, op->address, 0)) {i_hits++;} else {i_misses++;}
        // } 
        // else if(access_type == DATA_READ) {
        //     if(d_dc->mem_access(d_dc, op->address, 0)) {d_hits++;} else {d_misses++;}
        // } 
        // else if(access_type == DATA_WRITE) {
        //     d_dc->mem_access(d_dc, op->address, 1);
        //     d_misses++;
        // }

        if(access_type == INSTRUCTION_READ) {
            if(i_kwc->mem_access(i_kwc, op->address, 0)) {i_hits++;} else {i_misses++;}
        } 
        else if(access_type == DATA_READ) {
            if(d_kwc->mem_access(d_kwc, op->address, 0)) {d_hits++;} else {d_misses++;}
        } 
        else if(access_type == DATA_WRITE) {
            d_kwc->mem_access(d_kwc, op->address, 1);
            d_misses++;
        }

        free(op);
    }

    free(line);

    fclose(f);

    float i_hit_rate = ((float)i_hits / (float)(i_hits + i_misses)) * 100;
    float i_miss_rate = ((float)i_misses / (float)(i_hits + i_misses)) * 100;

    printf("INSTRUCTION CACHE SUMMARY: %d HITS (%f%%), %d MISSES (%f%%)\n", i_hits, i_hit_rate, i_misses, i_miss_rate);

    float d_hit_rate = ((float)d_hits / (float)(d_hits + d_misses)) * 100;
    float d_miss_rate = ((float)d_misses / (float)(d_hits + d_misses)) * 100;

    printf("DATA CACHE SUMMARY: %d HITS (%f%%), %d MISSES (%f%%)\n", d_hits, d_hit_rate, d_misses, d_miss_rate);

    return 0;
}