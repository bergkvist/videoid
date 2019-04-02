#include "video_comparison.hpp"
#include <vector>
#include <math.h>

namespace {
    std::vector<double> rollingAvg(const std::vector<double> &data, const size_t windowSize) {
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

    std::vector<double> rollingStd(const std::vector<double> &data, const size_t windowSize) {
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

    std::vector<bool> findMatches(const std::vector<double> &avg, const std::vector<double> &std) {
        std::vector<bool> result(avg.size());
        for (size_t i = 0; i < avg.size(); ++i) {
            if (avg[i] > 0.9 && std[i] < 0.1) result[i] = true;
            else result[i] = false;
        }
        return result;
    }
}



ContentID::VideoComparison::VideoComparison(
    const ContentID::HashedVideo &asset, 
    const ContentID::HashedVideo &compilation
) {
    const double factor = 1.0 / (HASH_SIZE * HASH_SIZE);
    this->framerate = compilation.framerate;
    this->assetId = asset.videoid;
    this->compilationId = compilation.videoid;

    for (auto compilationFrame : compilation.frames) {
        int minDist = compilationFrame.hammingDistance(asset.frames[0]);
        for (auto assetFrame : asset.frames) {
            int dist = compilationFrame.hammingDistance(assetFrame);
            if (dist < minDist) minDist = dist;
        }
        this->signal.raw.push_back(1 - factor * minDist);
    }

    this->signal.movingAvg = ::rollingAvg(this->signal.raw, WINDOW_SIZE);
    this->signal.movingStd = ::rollingStd(this->signal.raw, WINDOW_SIZE);
    this->signal.binary = ::findMatches(this->signal.movingAvg, this->signal.movingStd);

    std::cout << "asset: " << this->assetId << " | compilation: "<< this->compilationId << " => ";
    for (size_t i = 0; i < this->signal.binary.size(); ++i) {
        if (this->signal.binary[i]) {
            const double seconds = (i + 60) / this->framerate;
            std::cout << "Match found: https://youtu.be/" << this->compilationId << "?t=" << seconds << "s\n";
            return;
        }
    }
    std::cout << "No matches found!\n";
}