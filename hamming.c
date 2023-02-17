#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define block unsigned short
#define bit bool

int decode(block input[]);
void printBlock(block i);
bit getBit(block b, int i);
int multipleXor(int *indicies, int len);

int main () {
    block bits = 0b1100111011110110;
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

    printf("%d\n", multipleXor(onList, onCount));
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
