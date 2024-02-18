#ifndef CACHE_H_
#define CACHE_H_

#include "operation.h"

typedef struct _DirectCacheLine
{
    int valid;
    int dirty; //only used in write back mode
    unsigned int tag;
} DirectCacheLine;

typedef struct _DirectCache
{
    enum Mode mode;
    int total_size;
    int block_size;
    int num_lines;
    int num_offset_bits;
    int num_index_bits;
    DirectCacheLine *lines;
    enum CacheReturn (*mem_access)(struct _DirectCache *self, unsigned int address, enum AccessType access_type);
} DirectCache;

DirectCache *init_direct_cache(int total_size, int block_size, enum Mode mode);

#endif