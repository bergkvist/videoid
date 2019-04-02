#include "hashed_frame.hpp"
#include <string>
#include <vector>

namespace ContentID {
    class HashedVideo {
        public:
            double framerate;
            std::string videoid;
            std::vector<ContentID::HashedFrame> frames;
            HashedVideo(std::string videoid);
    };
}