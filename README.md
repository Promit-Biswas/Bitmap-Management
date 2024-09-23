# Bitmap Management Project

## Overview

The Bitmap Management project aims to implement a bitmap data structure in C that allows for efficient storage and manipulation of values ranging from `0` to `2^32 - 1`. This project involves creating a dynamic array to represent the bitmap and includes functionalities for adding, deleting, and manipulating values.

## Features

- Create and destroy a bitmap
- Add and remove values from the bitmap
- Print all values in the bitmap
- Clone a bitmap
- Perform bitwise operations (NOT, AND, OR)
- Parse a string to create a bitmap

## Data Structure

The bitmap is represented as a structure containing the following fields:

```c
struct bitmap {
    struct bitmap *bm_self;  // Pointer for validity check
    u32 max_value;           // Maximum value that can be stored
    u32 first_value;         // First bit set
    u32 last_value;          // Last bit set
    u32 numbers;             // Number of '1' bits in buf[]
    u32 buf_len;             // Length of the buffer
    u32 buf[0];              // Flexible array member for bitmap storage
};
```

##How to run and Compile

To run and Compile use this in linux

```bash
gcc main.c src/*.c -o bitmap && ./bitmap
```
