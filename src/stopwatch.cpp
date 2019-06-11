#include <chrono>
#include "stopwatch.hpp"

void Stopwatch::start() {
	t1 = std::chrono::high_resolution_clock::now();
}

void Stopwatch::stop() {
	t2 = std::chrono::high_resolution_clock::now();
}

double Stopwatch::elapsed_time() {
	return std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() * 1.0e-9;
}