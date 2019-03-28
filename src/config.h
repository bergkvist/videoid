#pragma once
#include <string> 
#include <stdio.h>

#ifndef HASH_SIZE
    #define HASH_SIZE 32
#endif

#define BITS_IN_UINT 32
#define HASH_LENGTH HASH_SIZE * HASH_SIZE / BITS_IN_UINT
#define OUTPUT "images/out" + std::to_string(HASH_SIZE) + ".csv"