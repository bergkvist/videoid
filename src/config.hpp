#pragma once
#include <string> 
#include <stdio.h>

#define VERBOSE getenv("VERBOSE")

#ifndef HASH_SIZE
#define HASH_SIZE 32
#endif

#ifndef WINDOW_SIZE
#define WINDOW_SIZE 180
#endif

#define BITS_IN_UINT 32
#define HASH_LENGTH HASH_SIZE * HASH_SIZE / BITS_IN_UINT

#define MIN_MATCH_LENGTH 1.0 // seconds