#include "hashed_video.hpp"
#include <string>

ContentID::HashedVideo::HashedVideo(ContentID::Video video) : 
    videoid{video.videoid},
    framerate{video.capture.get(cv::CAP_PROP_FPS)}
{
    cv::Mat frame;
    while (video.capture.read(frame))
        this->frames.push_back(ContentID::HashedFrame{frame});
}