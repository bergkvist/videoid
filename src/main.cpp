#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "stopwatch.hpp"

#define BITS_IN_UINT 32
#define HASH_SIZE 64
#define HASH_LENGTH HASH_SIZE * HASH_SIZE / BITS_IN_UINT

unsigned int countSetBits(unsigned int n) { 
    unsigned int count = 0;
    while (n) {
        count += n & 1; 
        n >>= 1;
    }
    return count;
}

int SWAR(unsigned int i) {
    i = i - ((i >> 1) & 0x55555555);
    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
    return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

struct PHashFrame {
    unsigned int data[HASH_LENGTH];

    PHashFrame(cv::Mat frame) {
        cv::Size s(HASH_SIZE, HASH_SIZE);
        cv::resize(frame, frame, s);
        cv::cvtColor(frame, frame, cv::COLOR_RGB2GRAY);

        long int threshold = 0;

        for (size_t i = 0; i < BITS_IN_UINT * HASH_LENGTH; ++i)
            threshold += (long int) frame.data[i];
        threshold /= (BITS_IN_UINT * HASH_LENGTH);

        for (size_t i = 0; i < HASH_LENGTH; ++i) {
            this->data[i] = 0;
            for (size_t j = 0; j < BITS_IN_UINT; ++j)
                this->data[i] |= (frame.data[BITS_IN_UINT*i + j] > threshold) ? (1<<j) : 0;
        }    
    }

    size_t hammingDistance(const PHashFrame &h) {
        size_t count = 0;
        for (size_t i = 0; i < HASH_LENGTH; ++i) {
            count += SWAR(this->data[i] ^ h.data[i]);
        }
        return count;
    }
};

std::vector<PHashFrame> hashVideo(std::string filename) {
    cv::VideoCapture cap(filename);
    cv::Mat frame;
    
    std::vector<PHashFrame> result;
    while(cap.read(frame)) {
        PHashFrame hash(frame);
        result.push_back(hash);
    }
    return result;
}

int main (int argc, char** argv) {
    std::cout << "OpenCV version: " << CV_VERSION << std::endl;
    stopwatch t1, t2;

    t1.start();
    std::vector<PHashFrame> v1 = hashVideo("./videos/M_KWGJw6R24.mp4");
    std::vector<PHashFrame> v2 = hashVideo("./videos/ZTHsrEG5jhA.mp4");
    t1.stop();

    t2.start();
    //int i = 0;
    for (PHashFrame h1 : v1) {
        size_t minHamm = h1.hammingDistance(v2[0]);
        for (PHashFrame h2 : v2) {
            size_t hamm = h1.hammingDistance(h2);
            if (hamm < minHamm) minHamm = hamm;
        }
        //if (++i % 32 == 0) std::cout << std::endl;
        //std::cout << std::setw(4) << minHamm << " ";
    }
    t2.stop();

    std::cout 
        << "\n\n Elapsed time:\n  hash: " 
        << t1.elapsedTime() 
        << " s\n  comp: "
        << t2.elapsedTime()
        << " s\n";

    return 0;
}