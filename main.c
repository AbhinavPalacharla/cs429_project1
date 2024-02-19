#include "operation.h"
#include "cache.h"
#include "k_way_cache.h"
#include "fully_assoc_cache.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>


int main(int argc, char **argv) {
    if(argc < 2) {
        printf("EXEC FORMAT: ./a.out <TRACE_FILE.trace (FILE PATH)> <TOTAL CACHE SIZE (INT)> <BLOCK SIZE (INT)> <SET ASSOCIATIVITY (1, 2, 4, 8, 16, 32)>");
        exit(1);
    }

    FILE *f = fopen(argv[1], "r");

    if(f == NULL) {
        perror("Error opening file");
    }

    int total_cache_size = atoi(argv[2]);
    int block_size = atoi(argv[3]);
    int set_assoc = atoi(argv[4]);

    printf("TOTAL CACHE SIZE: %d | BLOCK SIZE: %d\n", total_cache_size, block_size);

    void *d_cache;
    void *i_cache;

    int i_hits = 0; int i_misses = 0;
    int d_hits = 0; int d_misses = 0;
    int total_accesses = 0;

    //initialize cache based on set associativity
    if(set_assoc == 1) {
        d_cache = init_direct_cache(total_cache_size, block_size, WRITE_BACK);
        i_cache = init_direct_cache(total_cache_size, block_size, WRITE_THROUGH);
    } else if(set_assoc == 32) {
        d_cache = init_fully_assoc_cache(total_cache_size, block_size, WRITE_BACK);
        i_cache = init_fully_assoc_cache(total_cache_size, block_size, WRITE_THROUGH);
    } else {
        d_cache = init_k_way_cache(total_cache_size, block_size, set_assoc, WRITE_BACK);
        i_cache = init_k_way_cache(total_cache_size, block_size, set_assoc, WRITE_THROUGH);
    }

    size_t len = 128; char *line = malloc(sizeof(char) * len);  ssize_t read;

    //read operations and process them
    while((read = getline(&line, &len, f)) != -1) {
        enum AccessType access_type;
        access_type = atoi(&(line[0]));

        memmove(line, line + 2, strlen(line) - 1);
        line[strlen(line) - 1] = '\0';
        char *hex = line;

        char *endptr;

        Operation *op = init_op(access_type, strtol(hex, &endptr, 16));

        total_accesses++;

        if(access_type == INSTRUCTION_READ) {
            if(set_assoc == 1) {
                if(((DirectCache *)i_cache)->mem_access(((DirectCache *)i_cache), op->address, access_type) == HIT) {i_hits++;} else {i_misses++;}
            } else if(set_assoc == 32) {
                if(((FullyAssocCache *)i_cache)->mem_access(((FullyAssocCache *)i_cache), op->address, access_type) == HIT) {i_hits++;} else {i_misses++;}
            } else {
                if(((KWayCache *)i_cache)->mem_access(((KWayCache *)i_cache), op->address, access_type) == HIT) {i_hits++;} else {i_misses++;}   
            }
        } else {
            if(set_assoc == 1) {
                if(((DirectCache *)d_cache)->mem_access(((DirectCache *)d_cache), op->address, access_type) == HIT) {d_hits++;} else {d_misses++;}
            } else if(set_assoc == 32) {
                if(((FullyAssocCache *)d_cache)->mem_access(((FullyAssocCache *)d_cache), op->address, access_type) == HIT) {d_hits++;} else {d_misses++;}
            } else {
                if(((KWayCache *)d_cache)->mem_access(((KWayCache *)d_cache), op->address, access_type) == HIT) {d_hits++;} else {d_misses++;}   
            }
        }
                
        free(op);
    }

    free(line);

    fclose(f);

    float i_hit_rate = ((float)i_hits / (float)(i_hits + i_misses)) * 100;
    float i_miss_rate = ((float)i_misses / (float)(i_hits + i_misses)) * 100;

    float d_hit_rate = ((float)d_hits / (float)(d_hits + d_misses)) * 100;
    float d_miss_rate = ((float)d_misses / (float)(d_hits + d_misses)) * 100;

    float total_missrate = (float)(i_misses + d_misses) / (float)total_accesses;

    float AMAT = (float) 1 + (float) total_missrate * 100;

    printf("\nSET ASSOCIATIVITY: %d\n\n", set_assoc);
    
    printf("INSTRUCTION CACHE HITS: %d (%f%%)\n", i_hits, i_hit_rate);
    printf("INSTRUCTION CACHE MISSES: %d (%f%%)\n", i_misses, i_miss_rate);
    printf("DATA CACHE HITS: %d (%f%%)\n", d_hits, d_hit_rate);
    printf("DATA CACHE MISSES: %d (%f%%)\n", d_misses, d_miss_rate);
    // printf("AMAT: %f\n", AMAT);

    printf("\n");

    return 0;
}