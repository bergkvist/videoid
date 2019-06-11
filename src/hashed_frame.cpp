#include "hashed_frame.hpp"
#include <opencv2/opencv.hpp>

namespace {
    // Count the number of set bits in the integer i
    inline int number_of_set_bits(unsigned int i) {
        /* Together with compiler flag -O3, this function offers a massive speed improvement */
        i = i - ((i >> 1) & 0x55555555);
        i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
        return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
    }
}

int VideoID::HashedFrame::hamming_distance(const HashedFrame &h) const {
    int count = 0;
    for (size_t i = 0; i < DATA_LENGTH; ++i) {
        count += ::number_of_set_bits(this->data[i] ^ h.data[i]);
    }
    return count;
}

VideoID::HashedFrame::HashedFrame() { }
VideoID::HashedFrame::HashedFrame(cv::Mat frame) {
    /* Benchmark (usage): 0.953 s */
    const cv::Size hash_size(HASH_SIZE, HASH_SIZE);
    cv::resize(frame, frame, hash_size);
    cv::cvtColor(frame, frame, cv::COLOR_RGB2GRAY);

    /* Benchmark (usage): 0.016 s */
    long int threshold = 0;

    for (size_t i = 0; i < PIXEL_COUNT; ++i)
        threshold += (long int) frame.data[i];
    threshold /= PIXEL_COUNT;

    /* Benchmark (usage): 0.095 s */
    // The idea here is that since each pixel turns into one bit, we can fit 32 such values in one uint.
    for (size_t i = 0; i < DATA_LENGTH; ++i) {
        this->data[i] = 0;
        for (size_t j = 0; j < PIXELS_PER_UINT; ++j)
            this->data[i] |= (frame.data[PIXELS_PER_UINT*i + j] > threshold) ? (1<<j) : 0;
    }
}
