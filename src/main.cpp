#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "stopwatch.hpp"

#define HASH_SIZE 64
#define HASH_LENGTH HASH_SIZE * HASH_SIZE / 8

unsigned int countSetBits(unsigned char n) { 
    unsigned int count = 0;
    while (n) {
        count += n & 1; 
        n >>= 1;
    }
    return count; 
}

struct PHashFrame {
    unsigned char data[HASH_LENGTH];

    PHashFrame(cv::Mat frame) {
        cv::Size s(HASH_SIZE, HASH_SIZE);
        cv::resize(frame, frame, s);
        cv::cvtColor(frame, frame, cv::COLOR_RGB2GRAY);

        long int threshold = 0;

        for (size_t i = 0; i < 8 * HASH_LENGTH; ++i)
            threshold += (long int) frame.data[i];
        threshold /= (8 * HASH_LENGTH);

        for (size_t i = 0; i < HASH_LENGTH; ++i) this->data[i] =
            ((frame.data[8*i + 0] > threshold) ? 0b00000001 : 0b00000000) |
            ((frame.data[8*i + 1] > threshold) ? 0b00000010 : 0b00000000) |
            ((frame.data[8*i + 2] > threshold) ? 0b00000100 : 0b00000000) |
            ((frame.data[8*i + 3] > threshold) ? 0b00001000 : 0b00000000) |
            ((frame.data[8*i + 4] > threshold) ? 0b00010000 : 0b00000000) |
            ((frame.data[8*i + 5] > threshold) ? 0b00100000 : 0b00000000) |
            ((frame.data[8*i + 6] > threshold) ? 0b01000000 : 0b00000000) |
            ((frame.data[8*i + 7] > threshold) ? 0b10000000 : 0b00000000);        
    }

    size_t hammingDistance(const PHashFrame &h) {
        size_t count = 0;
        for (size_t i = 0; i < HASH_LENGTH; ++i) {
            count += countSetBits(this->data[i] ^ h.data[i]);
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
    int i = 0;
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