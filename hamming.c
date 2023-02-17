#include "stdio.h"

char *block2bin(unsigned short i);

int main () {
    unsigned short bits = 0b0010101110001110;
    printf("bits: %s\n", block2bin(bits));
}