#ifndef FULLY_ASSOC_CACHE_H_
#define FULLY_ASSOC_CACHE_H_

typedef struct FullyAssocCacheLine
{
    int valid;
    unsigned int tag;
} FullyAssocCacheLine;

typedef struct _FullyAssocCache
{
    int num_lines;
    int block_size;
    int total_size;
    int num_offset_bits;
    int access_counter;
    int *access_history;
    FullyAssocCacheLine *lines;
    int (*mem_access)(struct _FullyAssocCache *self, unsigned int address, int write_miss);
} FullyAssocCache;

FullyAssocCache *init_fully_assoc_cache(int total_size, int block_size);

#endif