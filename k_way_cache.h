#ifndef K_WAY_CACHE_H_
#define K_WAY_CACHE_H_

typedef struct KWayCacheLine
{
    int valid;
    unsigned int tag;
} KWayCacheLine;

typedef struct _KWayCache
{
    int ways;
    int num_lines;
    int block_size;
    int access_counter;
    int *access_history;
    KWayCacheLine *lines;
    int (*mem_access)(struct _KWayCache *self, unsigned int address, int write_miss);
} KWayCache;

KWayCache *init_k_way_cache(int total_size, int block_size, int ways);

#endif