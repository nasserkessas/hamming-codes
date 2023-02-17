#include "stdio.h"
#include "stdlib.h"

#define block unsigned short

int decode(block input[] );
void printBlock(block i);

int main () {
    block bits = 0b0010101110001110;
    block input[1]  = {bits};
    decode(input);
    return 0;
}

int decode(block input[] ) {
    printBlock(input[0]);
}


void printBlock(block i) {
    size_t size = sizeof(short) * sizeof(char) * 8;
    size_t bit = size;

    char * str = malloc(size + 1);
    if(!str) return NULL;
    str[size] = 0;

    unsigned u = *(unsigned *)&i;
    for(; bit--; u >>= 1)
        str[bit] = u & 1 ? '1' : '0';

    for (size_t i = 0; i < size; i++) {
        putchar(str[i]);
        putchar(' ');
        if (i % 4 == 3) {
            putchar('\n');
        }
    }

    // for (int i = 0; i < sizeof(block)*2; i+=4) {
    //     for (int j=0; i<4; j++) {
    //         putchar(str[i+j]);
    //     }
    //     putchar('\n');

    // }
}