#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

// Definitions
#define block unsigned short    // 16 bits
#define bit bool                // 8 bits (only last is used)

// Function prototypes
void decode(block input[], int len, FILE *ptr);          // Function used to decode Hamming code
void encode(char *input, int len, FILE *ptr);            // Function used to encode plaintext
void printBlock(block i);                                // Function used to pretty print a block
bit getBit(block b, int i);                              // Function used to get a specific bit of a block
bit getCharBit(char b, int i);                           // Function used to get a specific bit of a char
block toggleBit(block b, int i);                         // Function used to toggle a specific bit of a block
block modifyBit(block n, int p, bit b);                  // Function used to modify a bit to a specific value
char modifyCharBit(char n, int p, bit b);                // Function used to modify a bit of a char to a specific value
int multipleXor(int *indicies, int len);                 // Function used to XOR all the elements of a list together (used to locate error and determine values of parity bits)
int inList(char **list, char *testString, int listLen);  // Function used to check if a test string is in a list 

void usage(char *path) {
    printf("\nUsage: %s [COMMANDS]\n\n\tencode \tEncodes plaintext to hamming code\n\t\t-i \"FILENAME\" \tReads plaintext from FILENAME (default is \"in.txt\")\n\t\t-o \"FILENAME\" \tOutputs hamming code to FILENAME (default is \"out.hm\")\n\n\tdecode \tDecodes hamming code and prints original plaintext\n\t\t-i \"FILENAME\" \tReads hamming code from FILENAME (default is \"out.hm\")\n\n", path);
}

int main (int argc, char **argv) {

    // Path of file //
    char *path = argv[0];

    // If no arguements given //
    if (argc == 1) {
        usage(path);
        return 1;
    }
    
    // Command //
    char *command = argv[1];

    // Check which command was given //
    if (strcmp(command, "decode") == 0) {
        
        // Default input filename //
        char rfilename[32] = "out.hm";

        // Default output filename //
        char wfilename[32] = "out.txt";

        // Check index of -i arguement //
        int inputIndex = inList(argv, "-i", argc);

        // If the an arguement is -i (input file) //
        if (inputIndex) {

            // Change read filename to given file //
            strcpy(rfilename, argv[inputIndex+1]);
        }

        // Check index of -o arguement //
        int outputIndex = inList(argv, "-o", argc);

        // If the an arguement is -o (output file) //
        if (outputIndex) {

            // Change read filename to given file //
            strcpy(wfilename, argv[outputIndex+1]);
        }

        FILE *rptr;
        FILE *wptr;

        // Check if input file exists and open file //
        if (!(rptr = fopen(rfilename,"r"))) {
            printf("File \"%s\" cannot be opened (does the file exist?). Aborting.\n", rfilename);
            return 1;
        }

        // Check if output file exists and open file //
        if (!(wptr = fopen(wfilename,"wb"))) {
            printf("File \"%s\" cannot be opened (does the file exist?). Aborting.\n", wfilename);
            return 1;
        }

        printf("Decoding file \"%s\" to \"%s\"\n", rfilename, wfilename);

        // Seek to end of file //
        fseek(rptr, 0L, SEEK_END);

        // Determine length of the file in bytes //
        int sz = ftell(rptr);

        // Go back to start of file //
        rewind(rptr);

        // Initialise hamming code input variable //
        block input[sz];

        // Read hamming code from file to variable //
        fread(input, sizeof(block), sz/sizeof(block), rptr);    


        decode(input, sz, wptr);
    }

    if (strcmp(command, "encode") == 0) {
        
        // Default output filename //
        char wfilename[32] = "out.hm";
        
        // Input filename //
        char rfilename[32] = "in.txt";

        // Whether the input is a file or not //
        bool fileInput = true;

        // Initialise file pointer variables //
        FILE *rptr;
        FILE *wptr;

        // initialise input variable //
        unsigned char *input;

        // initialise input size variable //
        int sz;


        // Check index of -o arguement //
        int outputIndex = inList(argv, "-o", argc);

        // If the an arguement is -o (output file) //
        if (outputIndex) {

            // Change write filename to given file //
            strcpy(wfilename, argv[outputIndex+1]);
        }

        // Check index of -i arguement //
        int inputIndex = inList(argv, "-i", argc);

        // If the an arguement is -i (input file) //
        if (inputIndex) {

            // Abort if both -t and -i options are present //
            if (inList(argv, "-t", argc)) {
                printf("-i (input file) and -t (text input) options cannot be used in conjunction. Aborting");
                return 1;
            }

            // Change read filename to given file //
            strcpy(rfilename, argv[inputIndex+1]);

            // Check if input file exists and open file //
            if (!(rptr = fopen(rfilename,"r"))) {
                printf("File \"%s\" cannot be opened (does the file exist?). Aborting.\n", rfilename);
                return 1;
            }

            printf("Encoding file \"%s\" to \"%s\"\n", rfilename, wfilename);
        }

        // Check index of -t arguement //
        int textIndex = inList(argv, "-t", argc);

        // If the an arguement is -t (text input) //
        if (textIndex) {

            fileInput = false;
            
            // Variable used to store current char when looping //
            char currentChar = argv[textIndex+1][0];

            // Variable used to store current char index when looping //
            int index = 0;

            // Loop to find length of input text (stop when null char encountered) //
            while (currentChar != '\0') {
                index++;
                currentChar = argv[textIndex+1][index];
            }

            // Make size equal to last index //
            sz = index;

            // Allocate sz+1 bytes of memory to store input //
            input = malloc(sz+1);

            // Copy text input to input variable //
            strcpy(input, argv[textIndex+1]);

            printf("Encoding \"%s\" to \"%s\"\n", input, wfilename);
        }

        if (fileInput) {
            // Seek to end of file //
            fseek(rptr, 0L, SEEK_END);

            // Determine length of the file in bytes //
            sz = ftell(rptr);

            // Go back to start of file //
            rewind(rptr);

            // Initialise hamming code input variable //
            input = malloc(sz+1);

            // Read hamming code from file to variable //
            fread(input, 1, sz, rptr);
        }

        // Open write file //
        wptr = fopen(wfilename,"wb");

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
    int blocks = ceil((float) len / messageBits);

    // Array of encoded blocks //
    block encoded[blocks+1];

    // Loop through each block //
    for (int i = 0; i < blocks+1; i++) {

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

            bit thisBit;
            
            if (i != blocks) {

                // Current overall bit number //
                int currentBit = i*messageBits + (j-skipped);
                
                // Current character //
                int currentChar = currentBit/(sizeof(char)*8); // int division
                
                // Value of current bit //
                thisBit = currentBit < len*sizeof(char)*8 ? getCharBit(input[currentChar], currentBit-currentChar*8) : 0;
            }

            else {
                thisBit = getBit(len/8, j-skipped+(sizeof(block)*8-messageBits));
            }

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
    fwrite(encoded, sizeof(block), blocks+1, ptr);
}

void decode(block input[], int len, FILE *ptr) {

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

    // Initialise output string //
    char output[len];

    // Amount of bits per block used to carry the message //
    int messageBits = bits - log2(bits) - 1;

    int currentBit, currentChar;

    int chars = 0;

    for (int i = 0; i < len/sizeof(block); i++) {

        // Initialise variable to store amount of parity bits passed //
        int skipped = 0;

        // Loop through each message bit in this block to populate final block //
        for (int j = 0; j < bits; j++) {

            // Skip bit if reserved for parity bit //
            if ((j & (j - 1)) == 0) { // Check if j is a power of two or 0
                skipped++;
                continue;
            }

            // Current overall bit number //
            currentBit = i*messageBits + (j-skipped);

            // Current character //
            currentChar = currentBit/(sizeof(char)*8); // int division

            if (currentBit-currentChar*8 == 0) {
                putchar(' ');
            }

            // Value of current bit //
            bit thisBit = getBit(input[i], j);

            printf("%d", thisBit);

            if (i != len/sizeof(block)-1) {
	        
                // Populate final decoded character //
                output[currentChar] = modifyCharBit(output[currentChar], currentBit-currentChar*sizeof(char)*8, thisBit);
            }

            else {
                chars = modifyBit(chars, j-skipped+(sizeof(block)*8-messageBits), thisBit);
            }

        }
    }

    printf("Chars: %d\n", chars);
    
    printf("\nDecoded hamming code: \"%s\"\n", output);

    fwrite(output, 1, chars, ptr);    
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

char modifyCharBit(char n, int p, bit b) {
    return ((n & ~(1 << (sizeof(char)*8-1-p))) | (b << (sizeof(char)*8-1-p)));
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

int inList(char **list, char *testString, int listLen) {
    for (int i = 0; i < listLen; i++) {
        if (strcmp(list[i], testString) == 0) {
            return i;
        }
    }
    return 0;
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
