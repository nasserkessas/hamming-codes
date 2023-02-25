# Hamming Code Encoder/Decoder

:computer: Work in progress :computer:

More functionality coming soon.

This C script encodes and decodes extended hamming code in the command line. Hamming codes are a type of error detection and correction code, the type of code implemented in this example, extended hamming codes, have to ability to correct one error per block and detect two errors per block. [See below](https://github.com/nasserkessas/hamming-codes/edit/master/README.md#Hamming-code-overview) for more background and how it works.

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

Hamming codes were invented in 1950 by Richard W. Hamming as a way of automatically correcting errors introduced by punched card readers. It makes use of strategically placed parity bits to detect up to two errors and correct up to one error.

This repository currently focuses on 16-11 hamming codes, so these will be used to demonstrate.

16-11 hamming codes use 16 bits (2 bytes) to store 11 message bits with 5 bits reserved for parity checks. The parity bits are in positions that are powers of two like the following.

|`0`|`1`|`2`| 3 |
|:-:|:-:|:-:|:-:|
|**`4`**| **5** | **6** | **7** |
|**`8`**| **9** | **10** | **11** |
| **12** | **13** | **14** | **15** |

Where `highlighted` positions are reserved parity bits.
