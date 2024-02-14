#ifndef CACHE_H_
#define CACHE_H_

typedef struct _DirectCacheLine
{
    int valid;
    unsigned int tag;
} DirectCacheLine;

typedef struct _DirectCache
{
    DirectCacheLine *lines;
    int (*mem_access)(struct _DirectCache *self, unsigned int address, int write_miss);
} DirectCache;

DirectCache *init_direct_cache(int total_size, int block_size);

#endif