#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define block unsigned short
#define bit bool

int decode(block input[]);
void printBlock(block i);
bit getBit(block b, int i);
block toggleBit(block b, int i);
int multipleXor(int *indicies, int len);

int main () {
    block bits = 0b1100101011110111; // Last bit flipped for testing
    block input[1]  = {bits};
    decode(input);
    return 0;
}

int decode(block input[] ) {
    int bits = sizeof(block) * 8;
    printBlock(input[0]);

    int onCount = 0;
    int onList[bits];
    for (int i = 1; i < bits; i++) {
	getBit(input[0], i);
	if (getBit(input[0], i)) {
        onList[onCount] = i;
	    onCount++;
	    }
    }

    int errorLoc = multipleXor(onList, onCount);

    if (errorLoc) {
        if (!(onCount & 1 ^ getBit(input[0], 0))) { // last bit of onCount (total parity) XOR first bit of block (parity bit)
            printf("\nMore than one error detected. Aborting.\n");
            return 1;
        } else {
            printf("\nDetected error at position %d, flipping bit.\n", errorLoc);
            input[0] = toggleBit(input[0], (bits-1) - errorLoc);
            printBlock(input[0]);
        }
    }
}

int multipleXor(int *indicies, int len) {
    int val = indicies[0];
    for (int i = 1; i < len; i++) {
	    val = val ^ indicies[i];
    }
    return val;
}

bit getBit(block b, int i) {
    return (b << i) & 0b1000000000000000;
}

block toggleBit(block b, int i) {
    return b ^ (1 << i);
}

void printBlock(block i) {
    size_t size = sizeof(block) * sizeof(char) * 8;
    size_t current_bit = size;

    char * str = malloc(size + 1);
    if(!str) return;
    str[size] = 0;

    unsigned u = *(unsigned *)&i;
    for(; current_bit--; u >>= 1)
        str[current_bit] = u & 1 ? '1' : '0';

    for (size_t i = 0; i < size; i++) {
        putchar(str[i]);
        putchar(' ');
        if (i % 4 == 3) {
            putchar('\n');
        }
    }
}
