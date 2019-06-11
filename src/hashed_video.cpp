#include "hashed_video.hpp"
#include <string>

VideoID::HashedVideo::HashedVideo(VideoID::Video video) : 
    videoid{video.videoid},
    framerate{video.capture.get(cv::CAP_PROP_FPS)}
{
    const int frame_count = video.capture.get(cv::CAP_PROP_FRAME_COUNT);
    this->frames.resize(frame_count);

    for (int i = 0; i < frame_count; ++i) {
        // benchmark: 0.000597313 s
        cv::Mat frame;
        // benchmark: 1.5391 s
        video.capture.read(frame);
        // benchmark: 0.3346 s
        this->frames[i] = VideoID::HashedFrame{frame};
    }
}
