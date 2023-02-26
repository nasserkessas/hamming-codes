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

## Background

Hamming codes were invented in 1950 by Richard W. Hamming as a way of automatically correcting errors introduced by punched card readers. It makes use of strategically placed parity bits to detect up to two errors and correct up to one error.

## Explanation

This repository currently focuses on 16-11 hamming codes, so these will be used to demonstrate.

16-11 hamming codes use 16 bits (2 bytes) to store 11 message bits with 5 bits reserved for parity checks. The parity bits are in positions that are powers of two like the following.

|`0`|`1`|`2`| 3 |
|:-:|:-:|:-:|:-:|
|**`4`**| **5** | **6** | **7** |
|**`8`**| **9** | **10** | **11** |
| **12** | **13** | **14** | **15** |

Where `highlighted` positions are reserved parity bits.

Each parity bit is repsonsible for its own subset of the block; the first bit (excluding position 0) is the parity bit for columns 2 and 4, the second for the last two columns, the third (position 4) is for rows 2 and 4, and the fourth (position 8) is for the last two rows.

The decoder (or reciever) can pinpoint the location of an error by essentially binary searching using information from each of the parity bits.

### Example

If the following block is recieved, the reciever can locate the error as follows.

|0|0|1|0|
|:-:|:-:|:-:|:-:|
|**1**|**0**|**1**|**1**|
|**1**|**0**|**1**|**0**|
|**1**|**1**|**1**|**0**|

 + The first parity check (bit 1) says that the message bits in the second and fourth columns have a parity of 0 (are even when added up), which they do, meaning the error is not in this subset.
 + The second parity check (bit 2) says that the message bits in the third and fourth columns have a parity of 1 (are odd when added up), which they do not, meaning the error is in this subset.
 + The third parity check (bit 4) says that the message bits in the second and fourth rows have a parity of 1 (are odd when added up), which they do, meaning the error is not in this subset.
 + Finally, the last parity check (bit 8) says that the message bits in the third and fourth rows have a parity of 1 (are odd when added up), which they do not, meaning the error is in this subset.

Therefore, the error must have been in position 10, and the decoder can flip this bit accordingly.

### Parity bit at position 0

The parity bit at position 0 is current unused as it cannot be used for message bits. However, extended hamming code makes use of this bit as an overall parity bit to detect cases with two errors. Therefore, if the subset parity bit checks find and error, but the overall parity bit says an error has not occured, there must have been two errors. However, hamming code is inable to detect more than 2 errors and incorrectly corrects the wrong bit in the case of an odd number of errors (3,5,7...) and detects a two error case with an even number of errors (2,4,6...).

## Hamming block sizes

As only one error can be corrected and two can be detected, when selecting a hamming block size, it is a tradeoff between accuracy as smaller blocks are able to correct all the errors and space as the smaller the block size, the higher the percentage of redundant bits (see table below). This is only an example not intended to be used in practise so smaller block sizes have been used.

| Block size (bits) | Redundant bits | Redundancy percentage |
|:-----------------:|:--------------:|:---------------------:|
|4                  |3               |75%                    |
|8                  |4               |50%                    |
|16                 |5               |31.25%                 |
|32                 |6               |18.75%                 |
|64                 |7               |10.94%                 |
|128                |8               |6.25%                  |
|256                |9               |3.52%                  |
|512                |10              |1.95%                  |
|...                |...             |...                    |
|1048576            |21              |0.002%                 |


## Using XORs

Setting the parity bits on the encoder's (sender's) end as well as detecting and locating an error on the decoder's (reciever's) end can be taken about another, much simpler way. The parity groups and bits positions were selected carefully and actually have much more meaning. The first parity group, if you look closer, is actually the group with a binary representation `___1` while the second is `__1_` and so on. So, by taking a big XOR (exclusive or; which outputs 1 if two bits are different) of all the bits' positions with a 1 (are "on"), we can spell out the error in binary due to this property.

### Example

Using the example from above, the bits that are "on" are bit 2 (`0010`), 4 (`0100`), 6 (`0110`), 7 (`0111`), 8 (`1000`), 10 (`1010`), 12 (`1100`), 13(`1101`) and 14 (`1110`). We can find the XOR of all of all these numbers (which is like finding the parity of each bit position) and the error location is spelled out in binary `1010`, which is the same 10 we found earlier!

![image](https://user-images.githubusercontent.com/59720679/221393768-acba6088-8d3f-47ff-9758-1c4ee7bffb89.png)

### Explanation

The reason this works is because by setting the parity bits, we are effectively making this equation equal to 0. When make a 0 a 1, we are adding it into the equation when we decode making use effectively XORing 0 and the error position, giving us the position. Likewise, when we make a 1 a 0, we are removing the position from the equation, which affects the parity by the bits of the position, once again spelling out the error.

### Setting parity bits with XORs

This means that we could use the same method to set the parity bits when encding. After inputing the message bits into the grid, skipping the parity bits' positions, we can just do the same thing, taking the XOR of all the positions, and the result will tell us the values of the parity bits we need to set. The first bit (highest value) of the result tells us what to set the parity bit governing the positions with the highest bit a one (`1___`) and so on.
