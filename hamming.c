#include "stdio.h"
#include "stdlib.h"

char *block2bin(unsigned short i);

int main () {
    unsigned short bits = 0b0010101110001110;
    printf("bits: %s\n", block2bin(bits));
}

char *block2bin(unsigned short i) {
    size_t bits = sizeof(short) * sizeof(char) * 8;

    char * str = malloc(bits + 1);
    if(!str) return NULL;
    str[bits] = 0;

    unsigned u = *(unsigned *)&i;
    for(; bits--; u >>= 1)
        str[bits] = u & 1 ? '1' : '0';

    return str;
}