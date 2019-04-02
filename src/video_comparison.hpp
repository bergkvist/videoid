#include "hashed_video.hpp"
#include <string>
#include <vector>

namespace ContentID {
    struct signal_t {
        std::vector<double> raw;
        std::vector<double> movingAvg;
        std::vector<double> movingStd;
        std::vector<bool> binary;
    };
    class VideoComparison {
        public:
            double framerate;
            std::string assetId;
            std::string compilationId;
            ContentID::signal_t signal;
            VideoComparison(const ContentID::HashedVideo &asset, const ContentID::HashedVideo &compilation);
    };
}