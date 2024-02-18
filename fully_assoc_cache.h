#ifndef FULLY_ASSOC_CACHE_H_
#define FULLY_ASSOC_CACHE_H_

#include "operation.h"
typedef struct FullyAssocCacheLine
{
    int valid;
    int dirty;
    unsigned int tag;
} FullyAssocCacheLine;

typedef struct _FullyAssocCache
{
    enum Mode mode;
    int num_lines;
    int block_size;
    int total_size;
    int num_offset_bits;
    int access_counter;
    int *access_history;
    FullyAssocCacheLine *lines;
    enum CacheReturn (*mem_access)(struct _FullyAssocCache *self, unsigned int address, enum AccessType access_type);
} FullyAssocCache;

FullyAssocCache *init_fully_assoc_cache(int total_size, int block_size, enum Mode mode);

#endif