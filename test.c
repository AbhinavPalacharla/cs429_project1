#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TOTAL_SIZE 1024
#define BLOCK_SIZE 32
#define WAYS 32

void printBinary(unsigned int address) {
    for (int i = sizeof(address) * 8 - 1; i >= 0; i--) {
        printf("%u", (address >> i) & 1);
    }
    printf("\n");
}

int main() {
    // unsigned int address = 0x408ed4;
    //BINARY: 10000001000111011010100

    unsigned int address = 0x10019d94;
    //BINARY: 10000000000011001110110010100
    //TAG: 1000000000001100111
    //INDEX:01100
    //OFFSET:10100

    int num_lines = TOTAL_SIZE / BLOCK_SIZE;

    int num_offset_bits = log2(BLOCK_SIZE); //2^n = number of addresses

    int num_index_bits = log2((num_lines / WAYS)); //need to index certain amount of lines but when theres k ways each index goes to k lines so divide by k

    unsigned int tag = address >> (num_offset_bits + num_index_bits); //tag is bits leftover in address without offset and index bits so right shift them off

    unsigned int offset_mask = (1 << num_offset_bits) - 1; //mask to get last num_offset_bits bits
    unsigned int offset = address & offset_mask;

    unsigned int index_mask = (1 << num_index_bits) - 1; //mask to get the num_index_bits bits
    unsigned int index = address >> num_offset_bits; //get rid of offset bits
    index &= index_mask; //isolate index bits

    printf("ADDRESS: %X\n", address);
    printf("TAG: %X\n", tag);
    printBinary(tag);

    printf("INDEX: %X\n", index);
    printBinary(index);

    printf("OFFSET: %X\n", offset);
    printBinary(offset);

    printf("\nNUM_LINES: %d\n", num_lines);
    printf("NUM_INDEX_BITS: %d\n", num_index_bits);
    printf("NUM_OFFSET_BITS: %d\n", num_offset_bits);

    return 0;
}