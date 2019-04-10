#include "video_comparison.hpp"
#include <vector>
#include <math.h>

namespace {
    std::vector<double> rolling_avg(const std::vector<double> &data, const size_t window_size) {
        const size_t output_size = data.size() - window_size;
        std::vector<double> result(output_size);
        for (size_t i = 0; i < output_size; ++i) {
            result[i] = 0;
            for (size_t j = 0; j < window_size; ++j) {
                result[i] += data[i+j];
            }
            result[i] /= window_size;
        }
        return result;
    }

    std::vector<double> rolling_std(const std::vector<double> &data, const size_t window_size) {
        const size_t output_size = data.size() - window_size;
        std::vector<double> result(output_size);
        std::vector<double> avg = rolling_avg(data, window_size);
        for (size_t i = 0; i < output_size; ++i) {
            result[i] = 0;
            for (size_t j = 0; j < window_size; ++j) {
                result[i] += std::pow(data[i+j] - avg[i], 2);
            }
            result[i] /= window_size;
            result[i] = std::sqrt(result[i]);
        }
        return result;
    }

    std::vector<bool> find_matches(
        const std::vector<double> &avg, 
        const std::vector<double> &std, 
        const double min_avg, 
        const double max_std
    ) {
        std::vector<bool> result(avg.size());
        for (size_t i = 0; i < avg.size(); ++i) {
            if (avg[i] > min_avg && std[i] < max_std) result[i] = true;
            else result[i] = false;
        }
        return result;
    }
}



ContentID::VideoComparison::VideoComparison(
    const ContentID::HashedVideo &asset, 
    const ContentID::HashedVideo &compilation
) 
  : framerate{ compilation.framerate }, 
    asset_id{ asset.videoid },
    compilation_id{ compilation.videoid },
    signal{ 0.8875 /*min avg*/, 0.07 /*max std*/ }
{
    constexpr double factor = 1.0 / (HASH_SIZE * HASH_SIZE);

    for (auto compilation_frame : compilation.frames) {
        int min_dist = compilation_frame.hamming_distance(asset.frames[0]);
        for (auto asset_frame : asset.frames) {
            int dist = compilation_frame.hamming_distance(asset_frame);
            if (dist < min_dist) min_dist = dist;
        }
        this->signal.raw.push_back(1 - factor * min_dist);
    }

    this->signal.moving_avg = ::rolling_avg(this->signal.raw, WINDOW_SIZE);
    this->signal.moving_std = ::rolling_std(this->signal.raw, WINDOW_SIZE);
    this->signal.binary = ::find_matches(
        this->signal.moving_avg, 
        this->signal.moving_std, 
        this->signal.min_avg, 
        this->signal.max_std
    );

    std::cout << "asset: " << this->asset_id << " | compilation: "<< this->compilation_id << " => ";
    for (size_t i = 0; i < this->signal.binary.size(); ++i) {
        if (this->signal.binary[i]) {
            const double seconds = (i + 60) / this->framerate;
            std::cout << "Match found: https://youtu.be/" << this->compilation_id << "?t=" << seconds << "s\n";
            return;
        }
    }
    std::cout << "No matches found!\n";
}