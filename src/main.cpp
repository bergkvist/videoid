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
    stopwatch t1, t2, t3, t4, t5;

    t1.start();
    ContentID::Video asset_video{argv[1]};
    t1.stop();

    t2.start();
    ContentID::Video compilation_video{argv[2]};
    t2.stop();

    t3.start();
    ContentID::HashedVideo asset_hash{asset_video};
    t3.stop();

    t4.start();
    ContentID::HashedVideo compilation_hash{compilation_video};
    t4.stop();

    t5.start();
    ContentID::VideoComparison comparison{asset_hash, compilation_hash};
    t5.stop();

    const std::string output_filename = "images/out" + std::to_string(HASH_SIZE) + ".csv";
    comparison.print_results();
    comparison.write_csv(output_filename);

    if (VERBOSE) std::cout 
        << "\nElapsed time:"
        << "\n Video (asset)                        " << t1.elapsed_time() << " s"
        << "\n Video (compilation)                  " << t2.elapsed_time() << " s"
        << "\n HashedVideo (asset)                  " << t3.elapsed_time() << " s"
        << "\n HashedVideo (compilation)            " << t4.elapsed_time() << " s"
        << "\n VideoComparison (asset, compilation) " << t5.elapsed_time() << " s"
        << "\n";

    return 0;
}

