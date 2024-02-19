import os

TRACE_FILE = "./traces/cc.trace"

TOTAL_CACHE_SIZE = 1024
BLOCK_SIZE = 32

assocs = [1, 2, 4, 8, 16, 32]
# assocs = [1, 32]
# assocs = [2, 4, 8, 16]

for set_assoc in assocs:
    os.system(f"./a.out {TRACE_FILE} {TOTAL_CACHE_SIZE} {BLOCK_SIZE} {set_assoc}")
