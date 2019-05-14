#include "hashed_frame.hpp"
#include "video.hpp"
#include <string>
#include <vector>

namespace ContentID {
    class HashedVideo {
        public:
            std::string videoid{"__INVALID__"};
            double framerate{-1};
            std::vector<ContentID::HashedFrame> frames;
            HashedVideo(ContentID::Video video);
    };
}