#include <iostream>
#include "video_comparison.hpp"
#include "stopwatch.hpp"

int main (int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Usage: compare-videos <asset-id> <compilation-id>\n";
        std::cout << "  <asset-id>:       The id of a YouTube video that you own\n";
        std::cout << "  <compilation-id>: The id of a YouTube video that you want to check\n";
        return -1;
    }

    if (VERBOSE) {
        std::cout << "OpenCV version: " << CV_VERSION << std::endl;
        std::cout << "HASH_SIZE=" << HASH_SIZE << std::endl;
    }
    stopwatch t1, t2, t3;

    t1.start();
    ContentID::HashedVideo asset_video{argv[1]};
    t1.stop();

    t2.start();
    ContentID::HashedVideo compilation_video{argv[2]};
    t2.stop();

    t3.start();
    ContentID::VideoComparison comparison{asset_video, compilation_video};
    t3.stop();

    const std::string output_filename = "images/out" + std::to_string(HASH_SIZE) + ".csv";
    comparison.print_results();
    comparison.write_csv(output_filename);

    if (VERBOSE) std::cout 
        << "\nElapsed time:"
        << "\n HashedVideo{asset}                  " << t1.elapsed_time() << " s"
        << "\n HashedVideo{compilation}            " << t2.elapsed_time() << " s"
        << "\n VideoComparison{asset, compilation} " << t3.elapsed_time() << " s"
        << "\n";

    return 0;
}

