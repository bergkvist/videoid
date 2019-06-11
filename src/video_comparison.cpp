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

    std::vector<bool> generate_binary_signal(
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

    ContentID::match_t find_first_match(std::vector<bool> binary_signal, size_t min_length) {
        ContentID::match_t result{ false, 0 };
        for (size_t i = 0; i < binary_signal.size(); ++i) {
            if (binary_signal[i]) {
                result.found = true;
                result.frame = i + WINDOW_SIZE/2;
                for (size_t j = 0; j < min_length; ++j) {
                    if (!binary_signal[i+j]) {
                        result.found = false;
                        result.frame = 0;
                        break;
                    }
                }
                if (result.found) break;
            }
        }
        return result;
    }
}

void ContentID::VideoComparison::print_results() {
    std::cout << "asset: " << this->asset_id << " | compilation: "<< this->compilation_id << " => ";
    if (this->match.found) {
        const int seconds = match.frame / this->framerate;
        std::cout << "Match found: https://youtu.be/" << this->compilation_id << "?t=" << seconds << "s\n";
    } else {
        std::cout << "No matches found\n";
    }
}

void ContentID::VideoComparison::write_csv(std::string output) {
    std::ofstream csv{output};
    std::ofstream cnf{output + ".ini"};

    cnf << "[metadata]\n"
        << "framerate = " << this->framerate << "\n"
        << "asset_id = " << this->asset_id << "\n"
        << "compilation_id = " << this->compilation_id << "\n";

    csv << "frame,raw,avg,std,bin,min_avg,max_std\n"; 
    for (size_t i = 0; i < this->signal.raw.size(); ++i) {
        csv << i 
            << "," << this->signal.raw[i];
        if (i > WINDOW_SIZE/2 && i < this->signal.raw.size() - WINDOW_SIZE/2) {
            csv << "," << this->signal.moving_avg[i-WINDOW_SIZE/2]
                << "," << this->signal.moving_std[i-WINDOW_SIZE/2]
                << "," << this->signal.binary[i-WINDOW_SIZE/2];
        } else {
            csv << ",,,";
        }
        csv << "," << this->signal.min_avg << "," << this->signal.max_std << "\n";
    }
}

ContentID::VideoComparison::VideoComparison(
    const ContentID::HashedVideo &asset, 
    const ContentID::HashedVideo &compilation
) 
  : framerate{ compilation.framerate }, 
    asset_id{ asset.videoid },
    compilation_id{ compilation.videoid },
    signal{ 0.8875 /*min_avg*/, 0.07 /*max_std*/ }
{
    constexpr double factor = 1.0 / (HASH_SIZE * HASH_SIZE);
    const int asset_length = asset.frames.size();
    const int compilation_length = compilation.frames.size();
    this->signal.raw.resize(compilation_length);
    
    int * min_hamming_distance = new int[compilation_length];
    std::fill_n(min_hamming_distance, compilation_length, HASH_SIZE * HASH_SIZE);

    #pragma omp parallel for reduction(min: min_hamming_distance[:compilation_length])
    for (int i = 0; i < compilation_length; ++i) {
        for (int j = 0; j < asset_length; ++j) {
            const int distance = compilation.frames[i].hamming_distance(asset.frames[j]);
            if (distance < min_hamming_distance[i]) min_hamming_distance[i] = distance;
        }
    }

    #pragma omp parallel for
    for (int i = 0; i < compilation_length; ++i)
        this->signal.raw[i] = 1 - factor * min_hamming_distance[i];
    delete [] min_hamming_distance;
    
    this->signal.moving_avg = ::rolling_avg(this->signal.raw, WINDOW_SIZE);
    this->signal.moving_std = ::rolling_std(this->signal.raw, WINDOW_SIZE);
    this->signal.binary = ::generate_binary_signal(this->signal.moving_avg, this->signal.moving_std, this->signal.min_avg, this->signal.max_std);
    this->match = ::find_first_match(this->signal.binary, MIN_MATCH_LENGTH / framerate);
}