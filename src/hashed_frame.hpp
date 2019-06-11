#pragma once
#include <opencv2/opencv.hpp>
#include "config.hpp"

namespace VideoID {
    constexpr int PIXEL_COUNT = HASH_SIZE * HASH_SIZE;
    constexpr int PIXELS_PER_UINT = 8 * sizeof(unsigned int);
    constexpr int DATA_LENGTH = PIXEL_COUNT / PIXELS_PER_UINT;

    class HashedFrame {
        private:
            unsigned int data[DATA_LENGTH];
        
        public:
            HashedFrame();
            HashedFrame(cv::Mat frame);
            int hamming_distance(const VideoID::HashedFrame &h) const;
    };
}
