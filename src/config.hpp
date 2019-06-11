#pragma once
#include <string> 
#include <stdio.h>

#define VERBOSE getenv("VERBOSE")

// When the image is resized, what is the number of pixels per side
#ifndef HASH_SIZE
#define HASH_SIZE 32
#endif

// This is used for computing rolling average/std
#ifndef WINDOW_SIZE 
#define WINDOW_SIZE 180
#endif

#ifndef _OPENMP
#define _OPENMP "NOT DETECTED!"
#endif

// What is the minium length requirement for a match?
#define MIN_MATCH_LENGTH 1.0 // seconds