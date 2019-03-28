#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "stopwatch.hpp"
#include "config.h"

// Count the number of set bits in the integer i
int SWAR(unsigned int i) {
    /* Together with compiler flag -O3, this function offers a massive speed improvement */
    i = i - ((i >> 1) & 0x55555555);
    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
    return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

std::vector<double> rollingAvg(std::vector<double> data, size_t windowSize) {
    const size_t outputSize = data.size() - windowSize;
    std::vector<double> result(outputSize);
    for (size_t i = 0; i < outputSize; ++i) {
        result[i] = 0;
        for (size_t j = 0; j < windowSize; ++j) {
            result[i] += data[i+j];
        }
        result[i] /= windowSize;
    }
    return result;
}

std::vector<double> rollingStd(std::vector<double> data, size_t windowSize) {
    const size_t outputSize = data.size() - windowSize;
    std::vector<double> result(outputSize);
    std::vector<double> avg = rollingAvg(data, windowSize);
    for (size_t i = 0; i < outputSize; ++i) {
        result[i] = 0;
        for (size_t j = 0; j < windowSize; ++j) {
            result[i] += std::pow(data[i+j] - avg[i], 2);
        }
        result[i] /= windowSize;
        result[i] = std::sqrt(result[i]);
    }
    return result;
}

struct HashedFrame {
    unsigned int data[HASH_LENGTH];

    HashedFrame(cv::Mat frame) {
        /* Benchmark (usage): 0.953 s */
        cv::Size s(HASH_SIZE, HASH_SIZE);
        cv::resize(frame, frame, s);
        cv::cvtColor(frame, frame, cv::COLOR_RGB2GRAY);

        /* Benchmark (usage): 0.016 s */
        long int threshold = 0;

        for (size_t i = 0; i < BITS_IN_UINT * HASH_LENGTH; ++i)
            threshold += (long int) frame.data[i];
        threshold /= (BITS_IN_UINT * HASH_LENGTH);

        /* Benchmark (usage): 0.095 s */
        for (size_t i = 0; i < HASH_LENGTH; ++i) {
            this->data[i] = 0;
            for (size_t j = 0; j < BITS_IN_UINT; ++j)
                this->data[i] |= (frame.data[BITS_IN_UINT*i + j] > threshold) ? (1<<j) : 0;
        }
    }

    int hammingDistance(const HashedFrame &h) {
        int count = 0;
        for (size_t i = 0; i < HASH_LENGTH; ++i) {
            count += SWAR(this->data[i] ^ h.data[i]);
        }
        return count;
    }
};

struct HashedVideo {
    double framerate;
    std::string videoid;
    std::vector<HashedFrame> frames;

    HashedVideo(std::string videoid) {
        cv::VideoCapture cap("./videos/" + videoid + ".mp4");
        cv::Mat frame;
        this->framerate = cap.get(cv::CAP_PROP_FPS);
        this->videoid = videoid;
        while (cap.read(frame)) {
            HashedFrame hash(frame);
            this->frames.push_back(hash);
        }
    }
};

struct VideoComparison {
    double framerate;
    std::string assetId;
    std::string compilationId;
    std::vector<double> data;

    VideoComparison(const HashedVideo &asset, const HashedVideo &compilation) {
        const double factor = 1.0 / (HASH_SIZE * HASH_SIZE);
        this->framerate = compilation.framerate;
        this->assetId = asset.videoid;
        this->compilationId = compilation.videoid;

        for (HashedFrame ch : compilation.frames) {
            int minDist = ch.hammingDistance(asset.frames[0]);
            for (HashedFrame assetFrame : asset.frames) {
                int dist = ch.hammingDistance(assetFrame);
                if (dist < minDist) minDist = dist;
            }
            this->data.push_back(1 - factor * minDist);
        }
    }

    void classify() {
        std::vector<double> a = rollingAvg(this->data, 120);
        std::vector<double> s = rollingStd(this->data, 120);
        std::vector<int> c(a.size());

        for (size_t i = 0; i < a.size(); ++i) {
            if (a[i] > 0.8 && s[i] < 0.05) {
                const int match = i / this->framerate + 1;
                std::cout << "\nMatch found: https://youtu.be/" << this->compilationId << "?t=" << match << "s\n";
                break;
            }
        }
    }
};

int main (int argc, char** argv) {
    std::cout << "OpenCV version: " << CV_VERSION << std::endl;
    std::cout << "HASH_SIZE=" << HASH_SIZE << " (output file: " << OUTPUT << ")" << std::endl;
    std::ofstream csv(OUTPUT);
    stopwatch t1, t2, t3;

    t1.start();
    HashedVideo v1("ZTHsrEG5jhA"); // asset
    HashedVideo v2("M_KWGJw6R24"); // compilation
    t1.stop();

    std::cout 
        << "\nVideos hashed:\n v1: " 
        << v1.framerate << " fps\n v2: " 
        << v2.framerate << " fps\n";

    t2.start();
    VideoComparison c(v1, v2);
    csv << "x\n"; for (double d : c.data) csv << d << std::endl;
    t2.stop();

    t3.start();
    c.classify();
    t3.stop();

    std::cout 
        << "\nElapsed time:\n hash: " 
        << t1.elapsedTime() 
        << " s\n comp: "
        << t2.elapsedTime()
        << " s\n find: "
        << t3.elapsedTime()
        << " s\n";

    return 0;
}