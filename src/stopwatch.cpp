#include <chrono>
#include "stopwatch.hpp"

void stopwatch::start() {
	t1 = std::chrono::high_resolution_clock::now();
}

void stopwatch::stop() {
	t2 = std::chrono::high_resolution_clock::now();
}

double stopwatch::elapsed_time() {
	return std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() * 1.0e-9;
}