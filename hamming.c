#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Definitions
#define block unsigned short // 16 bits
#define bit bool             // 8 bits (only last is used)

// Function prototypes
int decode(block input[]);               // Function used to decode Hamming code
void printBlock(block i);                // Function used to pretty print a block
bit getBit(block b, int i);              // Function used to get a specific bit of a block
block toggleBit(block b, int i);         // Function used to toggle a specific bit of a block
int multipleXor(int *indicies, int len); // Function used to XOR all the elements of a list together (used to locate error)


int main () {
    // Will change to file input in future //
    block bits = 0b1100101011110111; // Last bit flipped for testing
    block input[1]  = {bits}; // express input as an array of blocks
    
    decode(input);
    
    return 0;
}

int decode(block input[] ) {

    // Amount of bits in a block
    int bits = sizeof(block) * 8;

    // Print initial block //
    printBlock(input[0]);

    // Count of how many bits are "on"
    int onCount = 0;
    
    // Array of "on" bits
    int onList[bits];

    // Populate onCount and onList
    for (int i = 1; i < bits; i++) {
	getBit(input[0], i);
	if (getBit(input[0], i)) {
        onList[onCount] = i;
	    onCount++;
	    }
    }

    // Check for single errors //
    int errorLoc = multipleXor(onList, onCount);

    if (errorLoc) {
        
        // Check for multiple errors //
        if (!(onCount & 1 ^ getBit(input[0], 0))) { // last bit of onCount (total parity) XOR first bit of block (parity bit)
            printf("\nMore than one error detected. Aborting.\n");
            return 1;
        }
        
        // Flip error bit //
        else {
            printf("\nDetected error at position %d, flipping bit.\n", errorLoc);
            input[0] = toggleBit(input[0], (bits-1) - errorLoc);
            
            // Re-print block for comparison //
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
