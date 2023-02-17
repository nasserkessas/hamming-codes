#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define block unsigned short
#define bit bool

int decode(block input[]);
void printBlock(block i);
bit getBit(block b, int i);

int main () {
    block bits = 0b0010101110001110;
    block input[1]  = {bits};
    decode(input);
    return 0;
}

int decode(block input[] ) {
    printBlock(input[0]);
    int index = 0;
    printf("%d\n", getBit(input[0], index));
}

bit getBit(block b, int i) {
    return (b >> i) & 1;
}

void printBlock(block i) {
    size_t size = sizeof(short) * sizeof(char) * 8;
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