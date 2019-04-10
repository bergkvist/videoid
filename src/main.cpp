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

    std::ofstream csv{OUTPUT};
    stopwatch t1, t2;

    t1.start();
    ContentID::HashedVideo asset_video{argv[1]}; // asset
    ContentID::HashedVideo compilation_video{argv[2]}; // compilation
    t1.stop();

    t2.start();
    ContentID::VideoComparison comparison{asset_video, compilation_video};
    csv << "raw,avg,std,bin,min_avg,max_std\n"; 
    for (size_t i = 0; i < comparison.signal.raw.size(); ++i) {
        csv << comparison.signal.raw[i];
        if (i > WINDOW_SIZE/2 && i < comparison.signal.raw.size() - WINDOW_SIZE/2) {
            csv << "," << comparison.signal.moving_avg[i-WINDOW_SIZE/2]
                << "," << comparison.signal.moving_std[i-WINDOW_SIZE/2]
                << "," << comparison.signal.binary[i-WINDOW_SIZE/2];
        } else {
            csv << ",,,";
        }
        csv << "," << comparison.signal.min_avg << "," << comparison.signal.max_std << "\n";
    }
    t2.stop();

    if(VERBOSE) std::cout 
        << "\nElapsed time:\n hash: " 
        << t1.elapsed_time() 
        << " s\n comp: "
        << t2.elapsed_time()
        << " s\n";

    return 0;
}

