#include "hashed_frame.hpp"
#include "video.hpp"
#include <string>
#include <vector>

namespace VideoID {
    class HashedVideo {
        public:
            std::string videoid{"__INVALID__"};
            double framerate{-1};
            std::vector<VideoID::HashedFrame> frames;
            HashedVideo(VideoID::Video video);
    };
}