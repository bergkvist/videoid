#include "hashed_frame.hpp"
#include <string>
#include <vector>

namespace ContentID {
    class HashedVideo {
        public:
            double framerate{-1};
            std::string videoid{"__INVALID__"};
            std::vector<ContentID::HashedFrame> frames;
            HashedVideo(std::string videoid);
    };
}