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

    // int l1_sz = 1024; int l1_block_sz = 32; int l1_set_assoc = 2;
    // int l2_sz = 16384; int l2_block_sz = 128;

    int l1_total_sz = atoi(argv[2]); int l1_block_sz = atoi(argv[3]); int l1_set_assoc = atoi(argv[4]);
    int l2_total_sz = atoi(argv[5]); int l2_block_sz = atoi(argv[6]); int l2_set_assoc = atoi(argv[7]);

    // printf("TOTAL CACHE SIZE: %d | BLOCK SIZE: %d\n", total_cache_size, block_size);

    void *l1_d_cache;
    void *l1_i_cache;
    void *l2_cache;

    int l1_i_hits = 0; int l1_i_misses = 0;
    int l1_d_hits = 0; int l1_d_misses = 0;
    int l2_hits = 0; int l2_misses = 0;

    int total_accesses = 0;

    // initialize l1 caches based on set associativity
    if(l1_set_assoc == 1) {
        l1_d_cache = init_direct_cache(l1_total_sz, l1_block_sz, WRITE_BACK);
        l1_i_cache = init_direct_cache(l1_total_sz, l1_block_sz, WRITE_THROUGH);
    } else if(l1_set_assoc == 32) {
        l1_d_cache = init_fully_assoc_cache(l1_total_sz, l1_block_sz, WRITE_BACK);
        l1_i_cache = init_fully_assoc_cache(l1_total_sz, l1_block_sz, WRITE_THROUGH);
    } else {
        l1_d_cache = init_k_way_cache(l1_total_sz, l1_block_sz, l1_set_assoc, WRITE_BACK);
        l1_i_cache = init_k_way_cache(l1_total_sz, l1_block_sz, l1_set_assoc, WRITE_THROUGH);
    }

    //initialize l2 cache based on set associativity
    if(l2_set_assoc == 1) {
        l2_cache = init_direct_cache(l2_total_sz, l2_block_sz, WRITE_BACK);
    } else if(l2_set_assoc == 32) {
        l2_cache = init_fully_assoc_cache(l2_total_sz, l2_block_sz, WRITE_BACK);
    } else {
        l2_cache = init_k_way_cache(l2_total_sz, l2_block_sz, l2_set_assoc, WRITE_BACK);
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
            if(l1_set_assoc == 1) {
                if(((DirectCache *)l1_i_cache)->mem_access(((DirectCache *)l1_i_cache), op->address, access_type) == HIT) {
                    l1_i_hits++;
                } else {
                    l1_i_misses++; //mark as L1 miss

                    //check L2 cache
                    if(l2_set_assoc == 1) {
                        if(((DirectCache *)l2_cache)->mem_access(((DirectCache *)l2_cache), op->address, access_type) == HIT) {
                            l2_hits++;
                        } else {
                            l2_misses++;

                            //add line to L2 cache
                            // ((DirectCache *)l2_cache)->mem_access(((DirectCache *)l2_cache), op->address, INSTRUCTION_READ); //insert using read operation
                        }
                    } //TODO: handle other types of L2 cases

                    //add line to L1 cache
                    // ((DirectCache *)l1_i_cache)->mem_access(((DirectCache *)l1_i_cache), op->address, INSTRUCTION_READ); //insert using read op
                }

            } //TODO: handle other types of L1 cases
        } else {
            if(l1_set_assoc == 1) {
                if(((DirectCache *)l1_d_cache)->mem_access(((DirectCache *)l1_d_cache), op->address, access_type) == HIT) {
                    l1_d_hits++;
                } else {
                    l1_d_misses++;

                    //check L2 cache
                    if(l2_set_assoc == 1) {
                        if(((DirectCache *)l2_cache)->mem_access(((DirectCache *)l2_cache), op->address, access_type) == HIT) {
                            l2_hits++;
                        } else {
                            l2_misses++;
                        }
                    } //TODO: handle other types of L2 cases
                }
            }
        }

        // if(access_type == INSTRUCTION_READ) {
        //     if(set_assoc == 1) {
        //         if(((DirectCache *)i_cache)->mem_access(((DirectCache *)i_cache), op->address, access_type) == HIT) {i_hits++;} else {i_misses++;}
        //     } else if(set_assoc == 32) {
        //         if(((FullyAssocCache *)i_cache)->mem_access(((FullyAssocCache *)i_cache), op->address, access_type) == HIT) {i_hits++;} else {i_misses++;}
        //     } else {
        //         if(((KWayCache *)i_cache)->mem_access(((KWayCache *)i_cache), op->address, access_type) == HIT) {i_hits++;} else {i_misses++;}   
        //     }
        // } else {
        //     if(set_assoc == 1) {
        //         if(((DirectCache *)d_cache)->mem_access(((DirectCache *)d_cache), op->address, access_type) == HIT) {d_hits++;} else {d_misses++;}
        //     } else if(set_assoc == 32) {
        //         if(((FullyAssocCache *)d_cache)->mem_access(((FullyAssocCache *)d_cache), op->address, access_type) == HIT) {d_hits++;} else {d_misses++;}
        //     } else {
        //         if(((KWayCache *)d_cache)->mem_access(((KWayCache *)d_cache), op->address, access_type) == HIT) {d_hits++;} else {d_misses++;}   
        //     }
        // }
                
        free(op);
    }

    free(line);

    fclose(f);

    float l1_i_hit_rate = ((float)l1_i_hits / (float)(l1_i_hits + l1_i_misses)) * 100;
    float l1_i_miss_rate = ((float)l1_i_misses / (float)(l1_i_hits + l1_i_misses)) * 100;

    float l1_d_hit_rate = ((float)l1_d_hits / (float)(l1_d_hits + l1_d_misses)) * 100;
    float l1_d_miss_rate = ((float)l1_d_misses / (float)(l1_d_hits + l1_d_misses)) * 100;

    float l2_hit_rate = ((float)l2_hits / (float)(l2_hits + l2_misses)) * 100;
    float l2_miss_rate = ((float)l2_misses / (float)(l2_hits + l2_misses)) * 100;


    printf("L1 INSTRUCTION CACHE HITS: %d (%f%%)\n", l1_i_hits, l1_i_hit_rate);
    printf("L1 DATA CACHE HITS: %d (%f%%)\n", l1_d_hits, l1_d_hit_rate);
    printf("L2 CACHE HITS: %d (%f%%)\n", l2_hits, l2_hit_rate);

    printf("L1 INSTRUCTION CACHE MISSS: %d (%f%%)\n", l1_i_misses, l1_i_miss_rate);
    printf("L1 DATA CACHE MISSS: %d (%f%%)\n", l1_d_misses, l1_d_miss_rate);
    printf("L2 CACHE MISSS: %d (%f%%)\n", l2_misses, l2_miss_rate);

    // float i_hit_rate = ((float)i_hits / (float)(i_hits + i_misses)) * 100;
    // float i_miss_rate = ((float)i_misses / (float)(i_hits + i_misses)) * 100;

    // float d_hit_rate = ((float)d_hits / (float)(d_hits + d_misses)) * 100;
    // float d_miss_rate = ((float)d_misses / (float)(d_hits + d_misses)) * 100;

    // float total_missrate = (float)(i_misses + d_misses) / (float)total_accesses;

    // float AMAT = (float) 1 + (float) total_missrate * 100;

    // printf("\nSET ASSOCIATIVITY: %d\n\n", set_assoc);
    
    // printf("INSTRUCTION CACHE HITS: %d (%f%%)\n", i_hits, i_hit_rate);
    // printf("INSTRUCTION CACHE MISSES: %d (%f%%)\n", i_misses, i_miss_rate);
    // printf("DATA CACHE HITS: %d (%f%%)\n", d_hits, d_hit_rate);
    // printf("DATA CACHE MISSES: %d (%f%%)\n", d_misses, d_miss_rate);
    // printf("AMAT: %f\n", AMAT);

    printf("\n");

    return 0;
}