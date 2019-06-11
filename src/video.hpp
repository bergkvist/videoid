#include <opencv2/opencv.hpp>
#include <string>
#include "config.hpp"

namespace VideoID {
    class Video {
        public:
            std::string videoid{"__INVALID__"};
            cv::VideoCapture capture;
            Video(std::string videoid);
    };
}