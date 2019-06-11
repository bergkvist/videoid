#include "hashed_video.hpp"
#include <string>
#include <vector>

namespace VideoID {
    struct signal_t {
        double min_avg;
        double max_std;
        std::vector<double> raw;
        std::vector<double> moving_avg;
        std::vector<double> moving_std;
        std::vector<bool> binary;
    };
    struct match_t {
        bool found;
        size_t frame;
    };
    class VideoComparison {
        public:
            double framerate;
            std::string asset_id;
            std::string compilation_id;
            VideoID::signal_t signal;
            VideoID::match_t match;
            VideoComparison(const VideoID::HashedVideo &asset, const VideoID::HashedVideo &compilation);
            void print_results();
            void write_csv(std::string output);
    };
}