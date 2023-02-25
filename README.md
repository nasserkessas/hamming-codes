# Hamming Code Encoder/Decoder

:computer: Work in progress :computer:

More functionality coming soon.

This C script encodes and decodes extended hamming code in the command line. [See below](https://github.com/nasserkessas/hamming-codes/edit/master/README.md#Hamming-code-overview) for more background and how it works.

# Usage

## Build

```sh
gcc -o hamming hamming.c
```

## Run
```sh
./hamming
```

## Commands

### Encode

Encodes a file into hamming code.

#### Options

 + `-i "FILENAME"` [REQUIRED] - Input filename. Reads plaintext from FILENAME.
 + `-o "FILENAME"` [OPTIONAL] - Output filename. Outputs hamming code to FILENAME. Defaults to `out.hm`.
 
For example:

```sh
./hamming encode -i in.txt -o out.hm
```

### Decode

Decodes a hamming code file into plaintext.

#### Options

 + `-i "FILENAME"` [OPTIONAL] - Input filename. Reads hamming code from FILENAME. Defaults to `out.hm`.
 + `-o "FILENAME"` [OPTIONAL] - Output filename. Outputs plaintext to FILENAME. Defaults to `out.txt`.
 
 For example:

```sh
./hamming decode -i out.hm -o out.txt
```

# Hamming code overview


