#include "hashed_video.hpp"
#include <string>
#include <vector>

namespace ContentID {
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
            ContentID::signal_t signal;
            ContentID::match_t match;
            VideoComparison(const ContentID::HashedVideo &asset, const ContentID::HashedVideo &compilation);
            void print_results();
            void write_csv(std::string output);
    };
}