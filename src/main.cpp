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
        std::cout << "\n"
            << "OpenCV version   : " << CV_VERSION << "\n"
            << "OpenMP version   : " << _OPENMP << "\n"
            << "HASH_SIZE        : " << HASH_SIZE
            << "\t(This means the images/frames will be resized to "<< HASH_SIZE << "x" << HASH_SIZE <<") \n"
            << "WINDOW_SIZE      : " << WINDOW_SIZE
            << "\t(This is used for computing rolling average and rolling std)\n"
            << "MIN_MATCH_LENGTH : " << MIN_MATCH_LENGTH
            << "\t(in seconds. A match must last longer than this)\n\n";
    }
    Stopwatch t1, t2, t3, t4, t5;

    t1.start();
    VideoID::Video asset_video{argv[1]};
    t1.stop();

    t2.start();
    VideoID::HashedVideo asset_hash{asset_video};
    // OpenCV 3 only allows one stream to be open at a time, so we have to release.
    // NOTE: This is not needed when using OpenCV 4
    asset_video.capture.release();
    t2.stop();

    t3.start();
    VideoID::Video compilation_video{argv[2]};
    t3.stop();

    t4.start();
    VideoID::HashedVideo compilation_hash{compilation_video};
    compilation_video.capture.release();
    t4.stop();


    t5.start();
    VideoID::VideoComparison comparison{asset_hash, compilation_hash};
    t5.stop();

    const std::string output_filename = "images/out" + std::to_string(HASH_SIZE) + ".csv";
    comparison.print_results();
    comparison.write_csv(output_filename);

    if (VERBOSE) std::cout 
        << "\nElapsed time:"
        << "\n+-------------+---------+-----------------"
        << "\n| Resource    | Action  | Time"
        << "\n+-------------+---------+-----------------"
        << "\n| asset       | load    | " << t1.elapsed_time() << " s"
        << "\n| asset       | hash    | " << t2.elapsed_time() << " s"
        << "\n| compilation | load    | " << t3.elapsed_time() << " s"
        << "\n| compilation | hash    | " << t4.elapsed_time() << " s"
        << "\n| both        | compare | " << t5.elapsed_time() << " s"
        << "\n+-------------+---------+-----------------"
        << "\n";

    return 0;
}

