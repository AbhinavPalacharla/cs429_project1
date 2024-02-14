#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TOTAL_SIZE 1024
#define BLOCK_SIZE 32
#define WAYS 1

int main() {
    // unsigned int addr1 = 0x408ed4;
    //BINARY: 10000001000111011010100
    //TAG: 1000000100011
    //INDEX: 10110
    //OFFSET: 10100

    unsigned int address = 0x10019d94;
    //BINARY: 10000000000011001110110010100
    //TAG: 1000000000001100111
    //INDEX:01100
    //OFFSET:10100

    unsigned int num_offset_bits = log2(BLOCK_SIZE);

    
    unsigned int num_lines = TOTAL_SIZE / BLOCK_SIZE;

    unsigned int num_index_bits = log2(num_lines / WAYS);

    printf("num_offset_bits %d | %X\n", num_offset_bits, num_offset_bits);
    printf("num_index_bits %d | %X\n", num_index_bits, num_index_bits);

    //TAG
    unsigned int tag = address >> (num_offset_bits + num_index_bits);

    //INDEX
    // unsigned int sec_last_5_mask = 0x3E0;
    // unsigned int index = addr1 & sec_last_5_mask;
    // index >>= 5;
    unsigned int mask = (1 << num_index_bits) - 1;
    unsigned int index = (int) ((address >> num_offset_bits) & mask);


    //OFFSET
    // unsigned int last_5_mask = 0x1F;
    // unsigned int offset = addr1 & last_5_mask;
    unsigned int offset = address & mask;

    printf("ADDRESS: %X\n", address);
    printf("TAG: %X\n", tag);
    printf("INDEX: %X\n", index);
    printf("OFFSET: %X\n", offset);

    return 0;
}