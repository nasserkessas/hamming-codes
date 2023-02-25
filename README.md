# Hamming Code Encoder/Decoder

:computer: Work in progress :computer:

More functionality coming soon.

## Description

This C script can encode and decode to extended hamming code in the command line.

## Usage

### Build

```sh
gcc -o hamming hamming.c
```

### Run
```sh
./hamming
```

### Commands

#### Encode

Encodes a file into hamming code.

##### Options

 + `-i "FILENAME"` [REQUIRED] - Input filename. Reads plaintext from FILENAME.
 + `-o "FILENAME"` [OPTIONAL] - Output filename. Outputs hamming code to FILENAME. Defaults to `out.hm`.
 
For example:

```sh
./hamming encode -i in.txt -o out.hm
```

#### Decode

Decodes a hamming code file into plaintext.

##### Options

 + `-i "FILENAME"` [OPTIONAL] - Input filename. Reads hamming code from FILENAME. Defaults to `out.hm`.
 + `-o "FILENAME"` [OPTIONAL] - Output filename. Outputs plaintext to FILENAME. Defaults to `out.txt`.
 
 For example:

```sh
./hamming decode -i out.hm -o out.txt
```
