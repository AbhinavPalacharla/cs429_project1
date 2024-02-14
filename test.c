#include <stdio.h>
#include <stdlib.h>

int main() {
    // unsigned int addr1 = 0x408ed4;
    //BINARY: 10000001000111011010100
    //TAG: 1000000100011
    //INDEX: 10110
    //OFFSET: 10100

    unsigned int addr1 = 0x10019d94;
    //BINARY: 10000000000011001110110010100
    //TAG: 1000000000001100111
    //INDEX:01100
    //OFFSET:10100

    //TAG
    unsigned int tag = addr1 >> 10;

    //INDEX
    unsigned int sec_last_5_mask = 0x3E0;
    unsigned int index = addr1 & sec_last_5_mask;
    index >>= 5;

    //OFFSET
    unsigned int last_5_mask = 0x1F;
    unsigned int offset = addr1 & last_5_mask;

    printf("ADDR1: %d\n", addr1);
    printf("TAG: %X\n", tag);
    printf("INDEX: %X\n", index);
    printf("OFFSET: %X\n", offset);

    return 0;
}