#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

// Definitions
#define block unsigned short    // 16 bits
#define bit bool                // 8 bits (only last is used)

// Function prototypes
void decode(block input[], int len);                // Function used to decode Hamming code
void encode(char *input, int len, FILE *ptr);       // Function used to encode plaintext
void printBlock(block i);                           // Function used to pretty print a block
bit getBit(block b, int i);                         // Function used to get a specific bit of a block
bit getCharBit(char b, int i);                      // Function used to get a specific bit of a char
block toggleBit(block b, int i);                    // Function used to toggle a specific bit of a block
block modifyBit(block n, int p, bit b);             // Function used to modify a bit to a specific value
int multipleXor(int *indicies, int len);            // Function used to XOR all the elements of a list together (used to locate error and determine values of parity bits)


int main (int argc, char **argv) {

    // Path of file //
    char *path = argv[0];

    // If no arguements given //
    if (argc == 1) {
        return 1; // usage function later
    }
    
    // Command //
    char *command = argv[1];

    // Check which command was given //
    if (strcmp(command, "decode") == 0) {
        
        // Default input filename //
        char filename[32] = "out.hm";

        // If there is a 2nd arguement //
        if (argv[2] != NULL) {
	    
	    // If the second arguement is -i (input file) //
            if (strcmp(argv[2], "-i") == 0) {

                // Change read filename to given file //
                strcpy(filename, argv[3]);
            }
        }

        FILE *ptr;

        // Check if file exists and open file //
        if (fopen(filename,"rb")) {
            printf("File \"%s\" cannot be opened (does the file exist?). Aborting.\n", filename);
            return 1;
        }

        printf("Decoding file \"%s\"\n", filename);

        // Seek to end of file //
        fseek(ptr, 0L, SEEK_END);

        // Determine length of the file in bytes //
        int sz = ftell(ptr);

        // Go back to start of file //
        rewind(ptr);

        // Initialise hamming code input variable //
        block input[sz];

        // Read hamming code from file to variable //
        fread(input, sizeof(block), sz/sizeof(block), ptr);    

        decode(input, sz);
    }

    if (strcmp(command, "encode") == 0) {
        
        // Default output filename //
        char wfilename[32] = "out.hm";
        
        // Input filename //
        char rfilename[32];

        // If there is no 2nd arguement //
        if (argv[2] == NULL) {
            printf("Encode must have an input file");
            return 1;
        }

        // If the second arguement is -i (input file) //
        if (strcmp(argv[2], "-i") == 0) {

            // Change read filename to given file //
            strcpy(rfilename, argv[3]);
        }
        
        FILE *ptr;

        // Check if file exists and open file //
        if (ptr = fopen(rfilename,"r")) {
            printf("File \"%s\" cannot be opened (does the file exist?). Aborting.\n", rfilename);
            return 1;
        }

        printf("Encoding file \"%s\" to \"%s\"\n", rfilename, wfilename);

        // Seek to end of file //
        fseek(ptr, 0L, SEEK_END);

        // Determine length of the file in bytes //
        int sz = ftell(ptr);

        // Go back to start of file //
        rewind(ptr);

        // Initialise hamming code input variable //
        unsigned char input[sz];

        // Read hamming code from file to variable //
        fread(input, 1, sz, ptr);
	
        // Open file for writing //
        FILE *wptr = fopen(wfilename,"wb");

        encode(input, sz*8, wptr);
    }
    
    return 0;
}

void encode(char *input, int len, FILE *ptr) {

    // Amount of bits in a block //
    int bits = sizeof(block) * 8;

    // Amount of bits per block used to carry the message //
    int messageBits = bits - log2(bits) - 1;

    // Amount of blocks needed to encode message //
    int blocks = ceil(len / messageBits);

    // Array of encoded blocks //
    block encoded[blocks];

    // Loop through each block //
    for (int i = 0; i < blocks; i++) {

        printf("On Block %d:\n", i);

        // Final encoded block variable //
        block thisBlock = 0;
        
        // Amount of "skipped" bits (used for parity) //
        int skipped = 0;

        // Count of how many bits are "on" //
        int onCount = 0;
        
        // Array of "on" bits //
        int onList[bits];

        // Loop through each message bit in this block to populate final block //
        for (int j = 0; j < bits; j++) {

            // Skip bit if reserved for parity bit //
            if ((j & (j - 1)) == 0) { // Check if j is a power of two or 0
                skipped++;
                continue;
            }
            
            // Current overall bit number //
            int currentBit = i*messageBits + (j-skipped);
            
            // Current character //
            int currentChar = currentBit/(sizeof(char)*8); // int division
            
            // Value of current bit //
            bit thisBit = currentBit < len*sizeof(char)*8 ? getCharBit(input[currentChar], currentBit-currentChar*8) : 0;

            // If bit is "on", add to onList and onCount //
            if (thisBit) {
                onList[onCount] = j;
                onCount++;
            }
            
            // Populate final message block //
            thisBlock = modifyBit(thisBlock, j, thisBit);
        }

        // Calculate values of parity bits //
        block parityBits = multipleXor(onList, onCount);

        // Loop through skipped bits (parity bits) //
        for (int k = 1; k < skipped; k++) { // skip bit 0

            // If bit is "on", add to onCount
            if (getBit(parityBits, sizeof(block)*8-skipped+k)) {
                onCount++;
            }

            // Add parity bit to final block //
            thisBlock = modifyBit(thisBlock, (int) pow(2, skipped-k-1) , getBit(parityBits, sizeof(block)*8-skipped+k));
        }

        // Add overall parity bit (total parity of onCount) //
        thisBlock = modifyBit(thisBlock, 0, onCount & 1);

        // Output final block //
        printBlock(thisBlock);
        putchar('\n');

        // Add block to encoded blocks //
        encoded[i] = thisBlock;
    }
    
    // Write encoded message to file //
    fwrite(encoded, sizeof(block), blocks, ptr);
}

void decode(block input[], int len) {

    // Amount of bits in a block //
    int bits = sizeof(block) * 8;

    for (int b = 0; b < (len/sizeof(block)); b++) {

        printf("On Block %d:\n", b);

        // Print initial block //
        printBlock(input[b]);

        // Count of how many bits are "on" //
        int onCount = 0;
        
        // Array of "on" bits //
        int onList[bits];

        // Populate onCount and onList //
        for (int i = 1; i < bits; i++) {
        getBit(input[b], i);
        if (getBit(input[b], i)) {
            onList[onCount] = i;
            onCount++;
            }
        }

        // Check for single errors //
        int errorLoc = multipleXor(onList, onCount);

        if (errorLoc) {
            
            // Check for multiple errors //
            if (!(onCount & 1 ^ getBit(input[b], 0))) { // last bit of onCount (total parity) XOR first bit of block (parity bit)
                printf("\nMore than one error detected. Aborting.\n");
                exit(1);
            }
            
            // Flip error bit //
            else {
                printf("\nDetected error at position %d, flipping bit.\n", errorLoc);
                input[b] = toggleBit(input[b], (bits-1) - errorLoc);
                
                // Re-print block for comparison //
                printBlock(input[b]);
            }
        }
        
        putchar('\n');
    }
}

int multipleXor(int *indicies, int len) {
    int val = indicies[0];
    for (int i = 1; i < len; i++) {
	    val = val ^ indicies[i];
    }
    return val;
}

block modifyBit(block n, int p, bit b) {
    return ((n & ~(1 << (sizeof(block)*8-1-p))) | (b << (sizeof(block)*8-1-p)));
}

bit getBit(block b, int i) {
    return (b << i) & (int) pow(2, (sizeof(block)*8-1));
}

bit getCharBit(char b, int i) {
    return (b << i) & (int) pow(2, (sizeof(char)*8-1));
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
