#include <iostream>
#include "video_comparison.hpp"
#include "stopwatch.hpp"

int main (int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Usage: compare-videos [asset id] [compilation id]";
        return -1;
    }

    if (VERBOSE) {
        std::cout << "OpenCV version: " << CV_VERSION << std::endl;
        std::cout << "HASH_SIZE=" << HASH_SIZE << " (output file: " << OUTPUT << ")" << std::endl;
    }

    std::ofstream csv(OUTPUT);
    stopwatch t1, t2, t3;

    t1.start();
    ContentID::HashedVideo v1(argv[1]); // asset
    ContentID::HashedVideo v2(argv[2]); // compilation
    t1.stop();

    t2.start();
    ContentID::VideoComparison c(v1, v2);
    csv << "raw,avg,std,bin,min_avg,max_std\n"; 
    for (size_t i = 0; i < c.signal.raw.size(); ++i) {
        csv << c.signal.raw[i];
        if (i > WINDOW_SIZE/2 && i < c.signal.raw.size() - WINDOW_SIZE/2) {
            csv << "," << c.signal.movingAvg[i-WINDOW_SIZE/2]
                << "," << c.signal.movingStd[i-WINDOW_SIZE/2]
                << "," << c.signal.binary[i-WINDOW_SIZE/2];
        }else {
            csv << ",,,";
        }
        csv << "," << 0.9 << "," << 0.1 << "\n";
    }
    t2.stop();

    if(VERBOSE) std::cout 
        << "\nElapsed time:\n hash: " 
        << t1.elapsedTime() 
        << " s\n comp: "
        << t2.elapsedTime()
        << " s\n";

    return 0;
}

