#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <opencv2/opencv.hpp>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "stopwatch.hpp"
#include "config.h"

#define VERBOSE getenv("VERBOSE")

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

std::string videoFolder() {
    std::string homeDir = getenv("HOME")
        ? getenv("HOME") 
        : getpwuid(getuid())->pw_dir;
    return homeDir + "/.videos";
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
        this->videoid = videoid;
        const std::string vf = videoFolder();
        std::ifstream binExists(vf + "/" + this->videoid + ".asset");
        std::ifstream mp4Exists(vf + "/" + this->videoid + ".mp4"); 
        if (!binExists) {
            if(!mp4Exists) {
                const std::string command("youtube-dl -f 'bestvideo[height<=144][ext=mp4]' -o '" + vf + "/%(id)s.%(ext)s' " + this->videoid);
                if(VERBOSE)
                std::cout << command << "\n";
                std::system(command.c_str());
            }
            cv::VideoCapture cap(vf + "/" + videoid + ".mp4");
            cv::Mat frame;
            this->framerate = cap.get(cv::CAP_PROP_FPS);
            this->videoid = videoid;
            while (cap.read(frame)) {
                HashedFrame hash(frame);
                this->frames.push_back(hash);
            }

            //const float f = 3.14f;
            //std::ofstream ofile(vf+"/"+this->videoid+"/.asset", std::ios::binary);
            //ofile.write((char*) &f, sizeof(float));
        }
    }
};

struct VideoComparison {
    double framerate;
    std::string assetId;
    std::string compilationId;
    std::vector<double> data;
    std::vector<double> rAvg;
    std::vector<double> rStd;
    std::vector<int> c;

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

        this->rAvg = rollingAvg(this->data, WINDOW_SIZE);
        this->rStd = rollingStd(this->data, WINDOW_SIZE);
    }

    void classify() {
        std::cout << "asset: " << this->assetId << " | compilation: "<< this->compilationId << " => ";
        for (size_t i = 0; i < this->rAvg.size(); ++i) {
            if (this->rAvg[i] > 0.9 && this->rStd[i] < 0.05) {
                const int match = (i + 60) / this->framerate;
                std::cout << "Match found: https://youtu.be/" << this->compilationId << "?t=" << match << "s\n";
                return;
            }
        }
        std::cout << "No matches found!\n";
    }
};

int main (int argc, char** argv) {
    videoFolder();
    if (argc != 3) {
        std::cout << "Usage: compare-videos [asset id] [compilation id]";
        return -1;
    }
    if(VERBOSE) {
        std::cout << "OpenCV version: " << CV_VERSION << std::endl;
        std::cout << "HASH_SIZE=" << HASH_SIZE << " (output file: " << OUTPUT << ")" << std::endl;
    }
    std::ofstream csv(OUTPUT);
    stopwatch t1, t2, t3;

    t1.start();
    HashedVideo v1(argv[1]); // asset
    HashedVideo v2(argv[2]); // compilation
    t1.stop();

    t2.start();
    VideoComparison c(v1, v2);
    csv << "avg,std\n"; 
    for (size_t i = 0; i < c.rAvg.size(); ++i)
        csv << c.rAvg[i] << "," << c.rStd[i] << "\n";
    t2.stop();

    t3.start();
    c.classify();
    t3.stop();

    if(VERBOSE) std::cout 
        << "\nElapsed time:\n hash: " 
        << t1.elapsedTime() 
        << " s\n comp: "
        << t2.elapsedTime()
        << " s\n find: "
        << t3.elapsedTime()
        << " s\n";

    return 0;
}