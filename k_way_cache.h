#ifndef K_WAY_CACHE_H_
#define K_WAY_CACHE_H_

#include "operation.h"

typedef struct KWayCacheLine
{
    int valid;
    int dirty;
    unsigned int tag;
} KWayCacheLine;

typedef struct _KWayCache
{
    enum Mode mode;
    int ways;
    int num_lines;
    int block_size;
    int access_counter;
    int *access_history;
    int num_offset_bits;
    int num_index_bits;
    KWayCacheLine *lines;
    enum CacheReturn (*mem_access)(struct _KWayCache *self, unsigned int address, enum AccessType access_type);
} KWayCache;

KWayCache *init_k_way_cache(int total_size, int block_size, int ways, enum Mode mode);

#endif