#pragma once
#include <opencv2/opencv.hpp>
#include "config.hpp"

namespace ContentID {
    class HashedFrame {
        private:
            unsigned int data[HASH_LENGTH];
        
        public:
            HashedFrame(cv::Mat frame);
            int hammingDistance(const ContentID::HashedFrame &h);
    };
}
